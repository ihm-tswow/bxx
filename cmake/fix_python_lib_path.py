import sys
import os
import shutil

# todo: this hack is just getting dumber and dumber,
#       but I can't figure out a way to extract the library i want from find_package  without going insane.
#       So here, I give back a single library path that's most likely to be what cmake actually wants.
cmake_list = ""
for arg in sys.argv[1:]:
    if ("/" in arg or "\\" in arg) and len(cmake_list) == 0 and not "_d.lib" in arg:
        cmake_list += arg

print(cmake_list,end='')

if 'debug' in sys.argv:
    sys.exit(0)

libpath = sys.argv[1]
dirname = os.path.dirname(libpath)
filename = os.path.basename(libpath)
[file,ext] = filename.split('.')
debugpath = os.path.join(dirname,file+'_d.' + ext)
shutil.copy(libpath,debugpath)