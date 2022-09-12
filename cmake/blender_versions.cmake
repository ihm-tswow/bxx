# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - Blender Versions -
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

function(set_blender_version version)
  if(version STREQUAL "3.3.0")
    set(BLENDER_VERSION_SHORT "3.3" PARENT_SCOPE)
    set(PYTHON_VERSION "3.10.2" PARENT_SCOPE)
    set(PIP_DOWNLOAD "https://bootstrap.pypa.io/pip/get-pip.py" PARENT_SCOPE)
    set(BLENDER_LINUX_BIN "https://download.blender.org/release/Blender3.3/blender-3.3.0-linux-x64.tar.xz" PARENT_SCOPE)
    set(BLENDER_WINDOWS_BIN "https://download.blender.org/release/Blender3.3/blender-3.3.0-windows-x64.zip" PARENT_SCOPE)
  elseif(version STREQUAL "3.0.0")
    set(BLENDER_VERSION_SHORT "3.0" PARENT_SCOPE)
    set(PYTHON_VERSION "3.9.7" PARENT_SCOPE)
    set(PIP_DOWNLOAD "https://bootstrap.pypa.io/pip/get-pip.py" PARENT_SCOPE)
    set(BLENDER_LINUX_BIN "https://download.blender.org/release/Blender3.0/blender-3.0.0-linux-x64.tar.xz" PARENT_SCOPE)
    set(BLENDER_WINDOWS_BIN "https://download.blender.org/release/Blender3.0/blender-3.0.0-windows-x64.zip" PARENT_SCOPE)
  elseif(version STREQUAL "2.79b")
    set(BLENDER_VERSION_SHORT "2.79" PARENT_SCOPE)
    set(PYTHON_VERSION "3.5.3" PARENT_SCOPE)
    set(PIP_DOWNLOAD "https://bootstrap.pypa.io/pip/3.5/get-pip.py" PARENT_SCOPE)
    set(BLENDER_LINUX_BIN "https://download.blender.org/release/Blender2.79/blender-2.79a-linux-glibc219-x86_64.tar.bz2" PARENT_SCOPE)
    set(BLENDER_WINDOWS_BIN "https://download.blender.org/release/Blender2.79/blender-2.79b-windows64.zip" PARENT_SCOPE)
  else()
    message( FATAL_ERROR "Unsupported Blender version ${version}")
  endif()
endfunction()