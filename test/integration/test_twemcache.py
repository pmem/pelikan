from base import GenericTest
from base import GenericPmemTest

from os import listdir
import os
import unittest


def twemcache():
    suite = unittest.TestSuite()
    for fname in listdir('twemcache'):
        test = GenericTest()
        test.load('twemcache/' + fname)
        suite.addTest(test)
    for fname in listdir('twemcache'):
        test = GenericPmemTest()
        test.load('twemcache/' + fname)
        suite.addTest(test)

    return suite


if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(twemcache())
    if os.path.exists("/dev/shm/pmem"):
        os.remove("/dev/shm/pmem")
