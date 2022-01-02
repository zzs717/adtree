#! /usr/bin/env python

# System imports
from distutils.core import *
from distutils      import sysconfig

# Third-party modules - we depend on numpy for everything
import numpy

# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

# adtree extension module
_adtree = Extension("_adtree",
                   ["adtree.i","adtree.c"],
                   include_dirs = [numpy_include],
                   )

# ezrange setup
setup(  name        = "adtree function",
        description = "blah",
        author      = "James Cussens",
        version     = "1.0",
        ext_modules = [_adtree]
        )
