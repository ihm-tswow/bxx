import sys
import os
import shutil
if 'debug' in sys.argv:
    sys.exit(0)

libpath = sys.argv[1]
dirname = os.path.dirname(libpath)
filename = os.path.basename(libpath)
[file,ext] = filename.split('.')
debugpath = os.path.join(dirname,file+'_d.' + ext)
shutil.copy(libpath,debugpath)