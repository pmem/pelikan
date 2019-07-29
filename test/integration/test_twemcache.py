from base import GenericTest
from base import GenericPmemTest

import os
import unittest
import ConfigParser
import StringIO

def defineTest(suite, fname, test_type):
    test = test_type()
    test.load('twemcache/' + fname)
    suite.addTest(test)

def twemcache():
    suite = unittest.TestSuite()
    for fname in os.listdir('twemcache'):
        if not os.path.isdir('twemcache/' + fname):
            defineTest(suite, fname, GenericTest)
            defineTest(suite, fname, GenericPmemTest)

    return suite


if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(twemcache())

    with open('twemcache.conf') as f:
        file_content = StringIO.StringIO('[dummy_section]\n' + f.read())

    config = ConfigParser.ConfigParser()
    config.readfp(file_content)
    devpath = config.get('dummy_section','slab_datapool')

    if os.path.exists(devpath):
        os.remove(devpath)
