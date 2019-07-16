import argparse
from math import ceil, floor, log
import os

INSTANCES = 3
PREFIX = 'test'
PELIKAN_ADMIN_PORT = 9900
PELIKAN_SERVER_PORT = 12300
PELIKAN_SLAB_MEM = 4294967296
PELIKAN_ITEM_OVERHEAD = 48
KSIZE = 32
VSIZE = 32
THREAD_PER_SOCKET = 48
BIND_TO_CORES = False
BIND_TO_NODES = True
ENGINE = "twemcache"

def generate_config(instances, vsize, slab_mem, pmem_paths, engine):
  # create top-level folders under prefix
  try:
    os.makedirs('config')
  except:
    pass
  try:
    os.makedirs('log')
  except:
    pass

  nkey = int(ceil(1.0 * slab_mem / (vsize + KSIZE + PELIKAN_ITEM_OVERHEAD)))
  hash_power = int(ceil(log(nkey, 2)))
  item_size = vsize + KSIZE + PELIKAN_ITEM_OVERHEAD

  # create twemcache|slimcache config file(s)
  for i in range(instances):
    admin_port = PELIKAN_ADMIN_PORT + i
    server_port = PELIKAN_SERVER_PORT + i
    config_file = '{engine}-{server_port}.config'.format(server_port=server_port, engine=engine)
    config_str = """\
daemonize: yes
admin_port: {admin_port}
server_port: {server_port}

admin_tw_cap: 2000

buf_init_size: 4096

buf_sock_poolsize: 16384

debug_log_level: 5
debug_log_file: log/{engine}-{server_port}.log
debug_log_nbuf: 1048576

klog_file: log/{engine}-{server_port}.cmd
klog_backup: log/{engine}-{server_port}.cmd.old
klog_sample: 100
klog_max: 1073741824

prefill: yes
prefill_ksize: 32
prefill_vsize: {vsize}
prefill_nkey: {nkey}

request_poolsize: 16384
response_poolsize: 32768

time_type: 2
""".format(admin_port=admin_port, server_port=server_port, vsize=vsize, nkey=nkey, engine=engine)

    pmem_path_str = ""
    datapool_param = ""
    if engine == "slimcache":
        datapool_param = "cuckoo_datapool"
        engine_str = """\

cuckoo_item_size: {item_size}
cuckoo_nitem: {nkey}
""".format(item_size=item_size, nkey=nkey)

    # else use twemcache settings
    else:
        datapool_param = "slab_datapool"
        engine_str = """\

slab_evict_opt: 1
slab_prealloc: yes
slab_hash_power: {hash_power}
slab_mem: {slab_mem}
slab_size: 1048756
slab_datapool_prefault: yes

stats_intvl: 10000
stats_log_file: log/{engine}-{server_port}.stats
""".format(hash_power=hash_power, slab_mem=slab_mem, engine=engine, server_port=server_port)

    if len(pmem_paths) > 0:
        pmem_path_str = """\

{datapool_param}: {pmem_path}
""".format(pmem_path=pmem_paths[i%len(pmem_paths)], datapool_param=datapool_param)

    config_str = config_str + engine_str + pmem_path_str
    with open(os.path.join('config', config_file),'w') as the_file:
      the_file.write(config_str)

def generate_runscript(binary, instances, engine, pmem_paths):
  # create bring-up.sh
  fname = 'bring-up.sh'
  with open(fname, 'w') as the_file:
    for i in range(instances):
      config_file = os.path.join('config', '{engine}-{server_port}.config'.format(server_port=PELIKAN_SERVER_PORT+i, engine=engine))
      if BIND_TO_NODES:
        the_file.write('sudo numactl --cpunodebind={numa_node} --preferred={numa_node} '.format(
            numa_node=i%len(pmem_paths)))
      elif BIND_TO_CORES:
        the_file.write('sudo numactl --physcpubind={physical_thread},{logical_thread} '.format(
            physical_thread=i,
            logical_thread=i+THREAD_PER_SOCKET))
      the_file.write('{binary_file} {config_file}\n'.format(
          binary_file=binary,
          config_file=config_file))
  os.chmod(fname, 0777)

  # create warm-up.sh
  fname = 'warm-up.sh'
  search_text = "prefilling cuckoo" if engine == "slimcache" else "prefilling slab"
  with open(fname, 'w') as the_file:
    the_file.write("""
./bring-up.sh

nready=0
while [ $nready -lt {instances} ]
do
    nready=$(grep -l "{search_text}" log/{engine}-*.log | wc -l)
    echo "$(date): $nready out of {instances} servers are warmed up"
    sleep 10
done
""".format(instances=instances, engine=engine, search_text=search_text))
  os.chmod(fname, 0777)


if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="""
    Generate all the server-side scripts/configs needed for a test run.
    """)
  parser.add_argument('--binary', dest='binary', type=str, help='location of pelikan_twemcache|pelikan_slimcache binary', required=True)
  parser.add_argument('--prefix', dest='prefix', type=str, default=PREFIX, help='folder that contains all the other files to be generated')
  parser.add_argument('--instances', dest='instances', type=int, default=INSTANCES, help='number of instances')
  parser.add_argument('--vsize', dest='vsize', type=int, default=VSIZE, help='value size')
  parser.add_argument('--slab_mem', dest='slab_mem', type=int, default=PELIKAN_SLAB_MEM, help='total capacity of slab memory, in bytes')
  parser.add_argument('--pmem_paths', dest='pmem_paths', nargs='*', help='a list of pmem mount points')
  parser.add_argument('--engine', dest='engine', type=str, default=ENGINE, help='a type of engine: twemcache or slimcache')

  args = parser.parse_args()

  if not os.path.exists(args.prefix):
    os.makedirs(args.prefix)
  os.chdir(args.prefix)

  generate_config(args.instances, args.vsize, args.slab_mem, args.pmem_paths, args.engine)
  generate_runscript(args.binary, args.instances, args.engine, args.pmem_paths)
