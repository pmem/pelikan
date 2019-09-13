set(test_name integration)

include(FindPythonInterp)

if(USE_PMEM)
    add_test(NAME ${test_name}
         COMMAND ${PYTHON_EXECUTABLE} test_twemcache.py --pmem
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    add_test(NAME ${test_name}
         COMMAND ${PYTHON_EXECUTABLE} test_twemcache.py
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif(USE_PMEM)
