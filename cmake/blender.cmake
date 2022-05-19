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
elseif(${BLENDER_VERSION} EQUAL "2.79b")
  set(PYTHON_VERSION "3.5.3")
else()
  message( FATAL_ERROR "Invalid Blender version ${BLENDER_VERSION}")
endif()