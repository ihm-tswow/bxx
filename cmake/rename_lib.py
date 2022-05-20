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
    raise Exception(f'Unsupported platform {platform.system()}')

lib_dir = os.path.join(source_root,'lib')
if not os.path.exists(lib_dir):
    os.mkdir(lib_dir)
lib_name = os.path.basename(build_file)[prefix_len:-len(ext)]

def write_lib():
    shutil.copyfile(build_file,os.path.join(lib_dir,lib_name + '-' + blender_version + ext))

if os.path.exists(AUTO_RELOAD_CONFIG_FILE):
    try:
        # the library automatically appends ".lock"
        with lock.FileLock(AUTO_RELOAD_LOCK_FILE,5):
            write_lib()
    except e:
        print(
            f"Failed to acquire autoload lock, cannot safely write library.",
            f"\n",
            f"\n",
            f"Please shut down blender, manually delete {AUTO_RELOAD_LOCK_FILE} and try again."
        )
else:
    write_lib()