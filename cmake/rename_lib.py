import os, sys
import shutil
import platform
# hack, but alternative is worse
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
import third_party.lock
from common.auto_reload import AUTO_RELOAD_CONFIG_FILE, AUTO_RELOAD_LOCK_FILE

[
    source_root,
    blender_version,
    build_type,
    build_file,
] = sys.argv[1:]

if platform.system() == 'Windows':
    prefix_len = 0
    ext = '.dll'
elif platform.system() == 'Linux':
    prefix_len = 3
    ext = '.so'
else:
    raise Exception('Unsupported platform %s' % (platform.system()))

lib_dir = os.path.join(source_root,'lib')
if not os.path.exists(lib_dir):
    os.mkdir(lib_dir)
lib_name = os.path.basename(build_file)[prefix_len:-len(ext)]

pdb_source = build_file[prefix_len:-len(ext)] + ".pdb"
pdb_dest = os.path.join(lib_dir,os.path.basename(pdb_source))

def write_lib():
    shutil.copyfile(build_file,os.path.join(lib_dir,lib_name + ext))
    if platform.system() == 'Windows' and os.path.exists(pdb_source):
        shutil.copyfile(pdb_source,pdb_dest)

if os.path.exists(AUTO_RELOAD_CONFIG_FILE):
    try:
        # the library automatically appends ".lock"
        with lock.FileLock(AUTO_RELOAD_LOCK_FILE,5):
            write_lib()
    except e:
        print(e)
        print(
            "Failed to acquire autoload lock, cannot safely write library.",
            "\n",
            "\n",
            "Please shut down blender, manually delete {0} and try again.".format(AUTO_RELOAD_LOCK_FILE)
        )
else:
    write_lib()