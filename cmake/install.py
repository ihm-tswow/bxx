import sys
import os
import zipfile
import shutil
from pathlib import Path

source_dir = sys.argv[1]
install_dir = sys.argv[2]
zip_path = sys.argv[3]

if zip_path.endswith('.zip'):
    zip_path = zip_path[0:-len('.zip')]

if os.path.exists(install_dir) and os.path.samefile(source_dir,install_dir):
    install_dir = os.path.join(source_dir,'install')

if os.path.exists(install_dir):
    shutil.rmtree(install_dir)

if os.path.exists(zip_path):
    os.remove(zip_path)

ignored_patterns = ['.git/','.github/','test_output/','__pycache__/','/bxx/core/build/']
for root, dirs, files in os.walk(source_dir):
    for file in files:
        filepath = os.path.join(root,file)
        cmp_path = filepath.replace('\\','/')

        found = False
        for pattern in ignored_patterns:
            if pattern in cmp_path:
                found = True
                break

        if found or file.endswith('.load'):
            continue

        relpath = os.path.relpath(filepath,source_dir)
        install_path = os.path.join(install_dir,relpath)
        install_path_dir = os.path.dirname(install_path)
        if source_dir != install_dir:
            if not os.path.exists(install_path_dir):
                os.makedirs(install_path_dir)
            shutil.copy(filepath,install_path)

shutil.make_archive(zip_path,'zip',install_dir)