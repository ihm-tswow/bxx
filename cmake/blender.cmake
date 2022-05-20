# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - Blender Versions -
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

set(BLENDER_VERSIONS
    "3.0.0"
    "2.79b"
)

if(${BLENDER_VERSION} EQUAL "3.0.0")
  set(BLENDER_VERSION_SHORT "3.0")
  set(PYTHON_VERSION "3.9.7")
  set(PIP_DOWNLOAD "https://bootstrap.pypa.io/pip/get-pip.py")
  set(BLENDER_LINUX_BIN "https://download.blender.org/release/Blender3.0/blender-3.0.0-linux-x64.tar.xz")
elseif(${BLENDER_VERSION} EQUAL "2.79b")
  set(BLENDER_VERSION_SHORT "2.79")
  set(PYTHON_VERSION "3.5.3")
  set(PIP_DOWNLOAD "https://bootstrap.pypa.io/pip/3.5/get-pip.py")
  set(BLENDER_LINUX_BIN "https://download.blender.org/release/Blender2.79/blender-2.79a-linux-glibc219-x86_64.tar.bz2")
else()
  message( FATAL_ERROR "Invalid Blender version ${BLENDER_VERSION}")
endif()