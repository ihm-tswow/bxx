import sys
import os
import re

# This file will inject special code into our local boost headers so we can add pdb lookup paths

# Very hacky, but i'm not going to fork boost over this.

# The proper solution is figuring out how to inject this to whatever underlying system boost is reading at runtime instead.

CLASS_HOOK = "class debugging_symbols: boost::noncopyable"
INJECT_HOOK = "iclient->QueryInterface(__uuidof(IDebugSymbols), idebug.to_void_ptr_ptr())"
DETOUR_NAME = "bxx_append_library_path"

boost_path = sys.argv[1]
frame_msvc = os.path.join(boost_path,'boost','stacktrace','detail','frame_msvc.ipp')

def error(msg):
    raise Exception(msg + " (you probably changed boost version to an incompatible one, please update cmake/boost_inject_pdb_hack.py)")

if not os.path.exists(frame_msvc):
    error("could not find frame_msvc.ipp")

with open(frame_msvc,'r+') as f:
    contents = f.read()
    previous_patch = re.search("bxx_append_library_path",contents)

    if not previous_patch:
        class_loc = contents.find(CLASS_HOOK)
        if not class_loc:
            error("could not find 'class debugging_symbols' hook")

        patch_loc = contents.find(INJECT_HOOK)

        if not patch_loc:
            error("could not find inject location")

        contents = contents.replace(CLASS_HOOK,'void bxx_append_library_path(com_holder<::IDebugSymbols>& idebug);\n'+CLASS_HOOK)
        contents = contents.replace(INJECT_HOOK,'if('+INJECT_HOOK+') bxx_append_library_path(idebug)')

        f.seek(0)
        f.write(contents)
        f.truncate()