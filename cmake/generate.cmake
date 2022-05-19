# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - Version Processing -
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

set(BLENDER_VERSION "3.0.0" CACHE STRING "Blender version to build for")
include(bxx/cmake/blender.cmake)
set_property(CACHE BLENDER_VERSION PROPERTY STRINGS ${BLENDER_VERSIONS})
add_compile_definitions("BXX_${BLENDER_ID}")
add_compile_definitions(BLENDER_VERSION="${BLENDER_ID}")

set(BLENDER_DOWNLOAD "https://github.com/blender/blender/archive/refs/tags/v${BLENDER_VERSION}.zip")
set(PYTHON_WINDOWS_DEV_DOWNLOAD "https://www.nuget.org/api/v2/package/python/${PYTHON_VERSION}")
set(PYTHON_WINDOWS_BIN_DOWNLOAD "https://www.python.org/ftp/python/${PYTHON_VERSION}/python-${PYTHON_VERSION}-embed-amd64.zip")

string(SUBSTRING ${PYTHON_VERSION} 0 1 PYTHON_MAJOR_VERSION)
string(SUBSTRING ${PYTHON_VERSION} 2 1 PYTHON_MINOR_VERSION)
string(SUBSTRING ${PYTHON_VERSION} 4 1 PYTHON_MICRO_VERSION)
set(BLENDER_ID blender-${BLENDER_VERSION})
set(PYTHON_ID python-${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}.${PYTHON_MICRO_VERSION})

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - Dependencies -
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        master
)

FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
)

FetchContent_Declare(
  ${BLENDER_ID}
  URL ${BLENDER_DOWNLOAD}
)

if(WIN32)
  FetchContent_Declare(
    ${PYTHON_ID}
    URL ${PYTHON_WINDOWS_BIN_DOWNLOAD}
  )
  if(NOT ${PYTHON_ID}_POPULATED)
    message(STATUS "Installing python")
    FetchContent_Populate(${PYTHON_ID})
  endif()

  set(PYTHON_PATH ${${PYTHON_ID}_SOURCE_DIR})
  set(PYTHON_BIN ${PYTHON_PATH}/python.exe)
  file(DOWNLOAD
    https://bootstrap.pypa.io/get-pip.py
    "${PYTHON_PATH}/get-pip.py"
  )

  if(EXISTS ${PYTHON_PATH}/Lib/site-packages/pip)
  else()
    message(STATUS "Installing pip")
    execute_process(COMMAND ${PYTHON_BIN} ${PYTHON_PATH}/get-pip.py)
  endif()
  file(WRITE ${PYTHON_PATH}/python${PYTHON_MAJOR_VERSION}${PYTHON_MINOR_VERSION}._pth "python39.zip\n.\nLib\\site-packages")
  if(EXISTS ${PYTHON_PATH}/Lib/site-packages/Cython)
  else()
    message(STATUS "Installing cython")
    execute_process(COMMAND ${PYTHON_PATH}/python.exe -m pip install cython)
  endif()

  FetchContent_Declare(
    ${PYTHON_ID}_dev
    URL ${PYTHON_WINDOWS_DEV_DOWNLOAD}
  )
  if(NOT ${PYTHON_ID}_dev_POPULATED)
    message(STATUS "Installing python development files")
    FetchContent_Populate(${PYTHON_ID}_dev)
  endif()
  file(COPY ${${PYTHON_ID}_dev_SOURCE_DIR}/tools/libs DESTINATION ${${PYTHON_ID}_SOURCE_DIR})
  file(COPY ${${PYTHON_ID}_dev_SOURCE_DIR}/tools/include DESTINATION ${${PYTHON_ID}_SOURCE_DIR})
endif()

FetchContent_MakeAvailable(fmt json)

# avoid calling any blender cmake scripts
FetchContent_GetProperties(blender)
if(NOT ${BLENDER_ID}_POPULATED)
  FetchContent_Populate(${BLENDER_ID})
endif()

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - core build library-
#
#   just a dummy project to load/compile in IDE,
#   it won't be used by anything.
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
add_custom_target(
  core
  COMMAND
    ${PYTHON_BIN} setup.py build_ext --inplace
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/bxx/core
  SOURCES
    bxx/core/core_cxx.cpp
    bxx/core/core.pyx
    bxx/core/setup.py
)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - bxx library -
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
file(GLOB_RECURSE bxx_files bxx/bxx/* bxx/common/*)
add_library(
  bxx STATIC
  ${bxx_files}
)

target_include_directories(
  bxx PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/bxx/common
  ${${BLENDER_ID}_SOURCE_DIR}/source/blender
  ${${BLENDER_ID}_SOURCE_DIR}/source/blender/blenlib
)

target_link_libraries(
  bxx PUBLIC
  fmt::fmt-header-only
  nlohmann_json::nlohmann_json
)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - scripts -
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

function(generate_filetree files)
  foreach(source IN LISTS files)
      get_filename_component(source_path "${source}" PATH)
      string(REPLACE "/" "\\" source_path_msvc "${source_path}")
      source_group("${source_path_msvc}" FILES "scripts\\${source}")
  endforeach()
endfunction()

set(scripts_core ${CMAKE_CURRENT_SOURCE_DIR}/bxx/scripts/script_core.cpp)
source_group("refs" FILES "${CMAKE_CURRENT_SOURCE_DIR}\\CMakeLists.txt" ${scripts_core})

file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/scripts ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*)
foreach(child ${children})
  set(full "${CMAKE_CURRENT_SOURCE_DIR}/scripts/${child}")
  if(IS_DIRECTORY ${full})
    file(GLOB_RECURSE script_files ${full}/*)
    file(GLOB_RECURSE script_files_rel RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/scripts ${full}/*)
    set(OLD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
    add_library(${child} SHARED ${script_files} ${scripts_core})
    set_target_properties(${child} PROPERTIES RELWITHDEBINFO_POSTFIX -${BLENDER_VERSION})
    set_target_properties(${child} PROPERTIES RELEASE_POSTFIX -${BLENDER_VERSION})
    set_target_properties(${child} PROPERTIES DEBUG_POSTFIX -${BLENDER_VERSION})
    set_target_properties(${child} PROPERTIES MINSIZEREL_POSTFIX -${BLENDER_VERSION})
    target_include_directories(
      ${child} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/bxx/bxx
      ${CMAKE_CURRENT_SOURCE_DIR}/bxx/common
    )
    target_link_libraries(${child} PRIVATE bxx)
    generate_filetree(${script_files_rel})
  endif()
endforeach()

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bxx"
        DESTINATION "./"
          PATTERN "*/build" EXCLUDE
          PATTERN "*core.c" EXCLUDE
          PATTERN "*.git" EXCLUDE
        )

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/scripts" DESTINATION "./")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Debug" DESTINATION "./build" FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo" DESTINATION "./build" FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Release" DESTINATION "./build" FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/MinSizeRel" DESTINATION "./build" FILES_MATCHING PATTERN "*.dll")