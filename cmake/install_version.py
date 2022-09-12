import os
import shutil
import sys

install_dir = sys.argv[1]
addon_dir = sys.argv[2]
if os.path.exists(addon_dir):
    shutil.rmtree(addon_dir)
shutil.copytree(install_dir,addon_dir)