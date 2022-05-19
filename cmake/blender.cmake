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
  set(PYTHON_VERSION "3.9.7")
  set(PIP_DOWNLOAD "https://bootstrap.pypa.io/pip/get-pip.py")
elseif(${BLENDER_VERSION} EQUAL "2.79b")
  set(PYTHON_VERSION "3.5.3")
  set(PIP_DOWNLOAD "https://bootstrap.pypa.io/pip/3.5/get-pip.py")
else()
  message( FATAL_ERROR "Invalid Blender version ${BLENDER_VERSION}")
endif()