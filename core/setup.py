from setuptools import setup, Extension
from Cython.Build import cythonize
import os

source_files = [
    '../common/shared_functions.cpp',
    '../common/exec.cpp',
    '../common/preferences.cpp',
    'core_cxx.cpp',
    'core_cy.pyx',
]

include_dirs = [
    '../common',
]

extensions = [
    Extension("core_cy",
        source_files,
        extra_compile_args = ['/std:c++20']
    )
]

setup(
    name='BlenderCXX Core',
    ext_modules=cythonize(extensions),
    zip_safe=False,
    include_dirs=include_dirs
)