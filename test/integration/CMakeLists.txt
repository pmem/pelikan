set(test_name integration)

include(FindPythonInterp)

if(USE_PMEM)
    set(test_arg --pmem)
endif(USE_PMEM)

add_test(NAME ${test_name}
    COMMAND ${PYTHON_EXECUTABLE} test_twemcache.py ${test_arg}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

