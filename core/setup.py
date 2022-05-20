from setuptools import setup, Extension
from Cython.Build import cythonize
import os
import sys
import platform

python_include = ''
for arg in sys.argv:
    if arg.startswith('python-include='):
        python_include = arg[len('python-include='):]
        sys.argv.remove(arg)

source_files = [
    '../common/shared_functions.cpp',
    '../common/exec.cpp',
    '../common/preferences.cpp',
    'core_cxx.cpp',
    'core_cy.pyx',
]

include_dirs = [
    '../common',
    python_include,
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