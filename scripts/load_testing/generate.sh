#!/bin/bash

conn_configs=(100 1000 10000)
mem_configs=(4 8 16 32)
size_configs=(64 128 256 512 1024 2048)
instances=30
ksize=32
rate=100000
threads=2

# Initialize our own variables:
client=false
server=false
rpcperf="rpc-perf"
pelikan="pelikan_twemcache"
target="127.0.0.1"
pmem_paths=()
engine="twemcache"

show_help()
{
    echo 'generate.sh [-c [-r path/to/rpcperf] [-t target/serverIP]] [-s [-p path/to/pelikan]] [-m "path/to/pmem0 path/to/pmem1"]'
    echo 'Note that the first pmem path is bound to the first numa node, the second path is bound to the next numa node.'
}

get_args()
{
    while getopts ":p:r:t:m:e:csh" opt; do
        case "$opt" in
        c)  client=true
            ;;
        s)  server=true
            ;;
        p)  pelikan=$OPTARG
            ;;
        r)  rpcperf=$OPTARG
            ;;
        t)  target=$OPTARG
            ;;
        m)  pmem_paths=($OPTARG)
            ;;
        h)
            show_help
            exit 0
            ;;
        \?)
            echo "unrecognized option $opt"
            show_help
            exit 1
            ;;
        esac
    done
}

# pelikan configs
gen_pelikan()
{
    for size in "${size_configs[@]}"
    do
        vsize=$((size - ksize))
        for mem in "${mem_configs[@]}"
        do
            slab_mem=$((mem * 1024 * 1024 * 1024))
            prefix=pelikan_${size}_${mem}
            python server_config.py --prefix="$prefix" --binary="$pelikan" --instances="$instances" --slab_mem "$slab_mem" --vsize "$vsize" --pmem_paths ${pmem_paths[@]} --engine "$engine"
        done
    done
}

# rpc-perf configs
gen_rpcperf()
{
    for conn in "${conn_configs[@]}"
    do
        for size in "${size_configs[@]}"
        do
            vsize=$((size - ksize))
            for mem in "${mem_configs[@]}"
            do
                slab_mem=$((mem * 1024 * 1024 * 1024))
                prefix=rpcperf_${conn}_${size}_${mem}
                python client_config.py --prefix="$prefix" --binary="$rpcperf" --server_ip="$target" --instances="$instances" --rate="$rate" --connections="$conn" --vsize "$vsize" --slab_mem="$slab_mem" --threads="$threads"
            done
        done
    done
}

get_args "${@}"
if [[ $($pelikan --help | grep -ic slimcache) > 0 ]] ; then engine="slimcache" ; else engine="twemcache" ; fi
if [ "$client" = true ]; then
    gen_rpcperf
fi
if [ "$server" = true ]; then
    gen_pelikan
fi
