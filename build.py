import subprocess
import shutil
import sys
import os

blender_version_map = {
    "3.0": "3.9.7",
    "2.79": "3.5.3"
}

blender_version = "3.0"
for arg in sys.argv:
    if arg.startswith("--blender="):
        blender_version = arg[len('--blender='):]

if not blender_version in blender_version_map:
    raise Exception(f"Unsupported blender version {blender_version}")

blender_python_version = blender_version_map[blender_version]
cur_python_version = f"{sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}"

if blender_python_version != cur_python_version:
    raise Exception(f"Using wrong python version {cur_python_version}, blender version {blender_version} needs {blender_python_version}")

build_core = "--build-core" in sys.argv
build_scripts = '--build-scripts' in sys.argv
generate_scripts = '--generate-scripts' in sys.argv

if not build_core and not build_scripts and not generate_scripts:
    build_core = True
    build_scripts = True
    generate_scripts = True

if build_core:
    wd = os.getcwd()
    os.chdir('bxx/core')
    subprocess.run(['python','setup.py','build_ext','--inplace'])
    os.chdir(wd)

if generate_scripts:
    subprocess.run(['cmake',f'-S {os.getcwd()}', f'-B {os.getcwd()}/build'])

if build_scripts:
    subprocess.run(['cmake',f'--build',f'{os.getcwd()}/build','--config','RelWithDebInfo'])