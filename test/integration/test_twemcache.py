from base import GenericTest
from base import GenericPmemTest

import os
import unittest

def defineTest(suite, fname, test_type):
    test = type()
    test.load('twemcache/' + fname)
    suite.addTest(test)

def twemcache():
    suite = unittest.TestSuite()
    for fname in os.listdir('twemcache'):
        defineTest(suite, fname, GenericTest)
        defineTest(suite, fname, GenericPmemTest)

    return suite


if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(twemcache())
    if os.path.exists("/dev/shm/pmem"):
        os.remove("/dev/shm/pmem")
