# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - Build settings -
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

unset(CMAKE_BUILD_TYPE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if((NOT DEFINED CMAKE_BUILD_TYPE) OR ("${CMAKE_BUILD_TYPE}" STREQUAL ""))
  set(CMAKE_BUILD_TYPE "Debug")
endif()

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

set(BLENDER_ID blender-${BLENDER_VERSION})
set(PYTHON_ID python-${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}.${PYTHON_MICRO_VERSION})

string(REPLACE "." ";" PYTHON_VERSION_LIST ${PYTHON_VERSION})
list(GET PYTHON_VERSION_LIST 0 PYTHON_MAJOR_VERSION)
list(GET PYTHON_VERSION_LIST 1 PYTHON_MINOR_VERSION)
list(GET PYTHON_VERSION_LIST 2 PYTHON_MICRO_VERSION)

string(REPLACE "." ";" BLENDER_VERSION_LIST ${BLENDER_VERSION})

list(GET BLENDER_VERSION_LIST 0 BLENDER_MAJOR_VERSION)
list(GET BLENDER_VERSION_LIST 1 BLENDER_MINOR_VERSION)

string(REGEX REPLACE "[a-zA-Z]" "" BLENDER_MINOR_VERSION_SHORT ${BLENDER_MINOR_VERSION})
set(BLENDER_BIN_DIR ${BLENDER_MAJOR_VERSION}.${BLENDER_MINOR_VERSION_SHORT})

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
    "${PIP_DOWNLOAD}"
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

if(UNIX AND NOT APPLE)
  # use the python blender ships with
  set(BLENDER_BIN_ID blender-${BLENDER_VERSION}-bin)
  FetchContent_Declare(
    ${BLENDER_BIN_ID}
    URL ${BLENDER_LINUX_BIN}
  )
  if(NOT ${BLENDER_BIN_ID}_POPULATED)
    message(STATUS "Installing python (via blender)")
    FetchContent_Populate(${BLENDER_BIN_ID})
  endif()

  set(PYTHON_BASE_PATH "${${BLENDER_BIN_ID}_SOURCE_DIR}/${BLENDER_BIN_DIR}/python")
  set(PYTHON_PATH "${PYTHON_BASE_PATH}/bin")
  set(PYTHON_BIN "${PYTHON_PATH}/python${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}")
  file(DOWNLOAD
    "${PIP_DOWNLOAD}"
    "${PYTHON_PATH}/get-pip.py"
  )
  message(${PYTHON_PATH})

  # pip / cython
  if(EXISTS ${PYTHON_PATH}/pip)
  else()
    execute_process(COMMAND ${PYTHON_BIN} ${PYTHON_PATH}/get-pip.py)
  endif()
  if(EXISTS ${PYTHON_PATH}/cython)
  else()
    execute_process(COMMAND ${PYTHON_BIN} -m pip install cython)
  endif()

  # python source code (includes)
  set(PYTHON_SOURCE python-${PYTHON_VERSION})
  FetchContent_Declare(
    ${PYTHON_SOURCE}
    URL "https://www.python.org/ftp/python/${PYTHON_VERSION}/Python-${PYTHON_VERSION}.tgz"
  )
  if(NOT ${PYTHON_SOURCE}_POPULATED)
    message(STATUS "Installing python development files")
    FetchContent_Populate(${PYTHON_SOURCE})
  endif()
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
  ALL
  COMMAND
    ${PYTHON_BIN} setup.py build_ext --inplace python-include="${${PYTHON_SOURCE}_SOURCE_DIR}/Include"
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/bxx/core
  SOURCES
    bxx/core/core_cxx.cpp
    bxx/core/core_cy.pyx
    bxx/core/core.py
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
# - unit tests-
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
add_custom_target(
  generate_scripts
  ALL ${PYTHON_BIN} ${CMAKE_CURRENT_SOURCE_DIR}/bxx/cmake/generate_scripts.py
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}
  SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bxx/cmake/generate_scripts.py
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

set(scripts_core ${CMAKE_CURRENT_SOURCE_DIR}/bxx/core/core_script.cpp)
source_group("refs" FILES "${CMAKE_CURRENT_SOURCE_DIR}\\CMakeLists.txt" ${scripts_core})

file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/scripts ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*)
foreach(child ${children})
  set(full "${CMAKE_CURRENT_SOURCE_DIR}/scripts/${child}")
  if(IS_DIRECTORY ${full})
    file(GLOB_RECURSE script_files ${full}/*)
    file(GLOB_RECURSE script_files_rel
      RELATIVE
      ${CMAKE_CURRENT_SOURCE_DIR}/scripts
      ${full}/*
    )
    list(REMOVE_ITEM script_files_rel ${child}/calls.generated.cpp)
    set(OLD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
    if(EXISTS "${full}/calls.generated.cpp")
    else()
      file(TOUCH "${full}/calls.generated.cpp")
    endif()

    add_library(${child} SHARED
      ${script_files}
      ${scripts_core}
      ${full}/calls.generated.cpp
    )
    source_group("refs" FILES "${full}\\calls.generated.cpp")
    add_dependencies(${child} generate_scripts)

    add_custom_command(
      TARGET ${child}
      POST_BUILD
      COMMAND ${PYTHON_BIN} ${CMAKE_CURRENT_SOURCE_DIR}/bxx/cmake/rename_lib.py
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${BLENDER_VERSION}
        ${CMAKE_BUILD_TYPE}
        $<TARGET_FILE:${child}>
    )

    target_include_directories(
      ${child} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/bxx/bxx
      ${CMAKE_CURRENT_SOURCE_DIR}/bxx/common
      ${SCRIPT_INCLUDE_DIRECTORIES}
    )
    target_link_libraries(
      ${child} PRIVATE
      bxx
      ${SCRIPT_LIBRARIES}
    )
    generate_filetree("${script_files_rel}")
  endif()
endforeach()

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bxx"
  DESTINATION "./"
  PATTERN "*/build" EXCLUDE
  PATTERN "*core.c" EXCLUDE
  PATTERN "*.git" EXCLUDE
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/scripts"
  DESTINATION "./"
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
  DESTINATION "./"
  FILES_MATCHING
    PATTERN "*.dll"
    PATTERN "*.so"
    PATTERN "*.dylib"
)