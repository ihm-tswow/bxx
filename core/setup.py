from setuptools import setup, Extension
from Cython.Build import cythonize
import os
import sys
import platform

include_dirs = ['../']

for arg in sys.argv:
    if arg.startswith('includes='):
        include_dirs += arg[len('includes='):].split('|')
        sys.argv.remove(arg)

source_files = [
    '../common/shared_functions.cpp',
    '../common/exec.cpp',
    '../common/addon.cpp',
    '../common/util.cpp',
    '../common/script_error.cpp',
    '../common/script_index.cpp',
    'core_cxx.cpp',
    'core_cy.pyx',
]

extensions = [
    Extension("core_cy",
        source_files,
        extra_compile_args = [
            '-std=c++17' if platform.system() == 'Linux' else
            '/std:c++17'
        ]
    )
]

setup(
    name='BlenderCXX Core',
    ext_modules=cythonize(extensions),
    zip_safe=False,
    include_dirs=include_dirs
)