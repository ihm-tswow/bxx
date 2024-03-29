include(FetchContent)
include(bxx/cmake/blender_versions.cmake)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE PATH "Installation directory")
set(BXX_ZIP_INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/install.zip" CACHE PATH "Installation zip")

set(INSTALL_TEST_COMMANDS "")
string(REPLACE "\\" "/" ESCAPED_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
string(REPLACE "\\" "/" ESCAPED_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
string(REPLACE "\\" "/" ESCAPED_ZIP_INSTALL_PATH ${BXX_ZIP_INSTALL_PATH})
set(ESCAPED_PYTHON_BIN "")
get_filename_component(BXX_ADDON_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

option(RUN_TESTS_ON_INSTALL "Whether to run automatic tests when the addon is installed" ON)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - Global Dependencies -
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

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
  boost
  URL https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/boost_1_80_0.tar.gz
)

FetchContent_Declare(
  magic_enum
  GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
  GIT_TAG 1a57977ea3a286206b800e3e3fd79faa6f6e7404
)

message(STATUS "Installing bxx global dependencies")
FetchContent_MakeAvailable(fmt json magic_enum boost)
set_property(DIRECTORY ${fmt_SOURCE_DIR} PROPERTY EXCLUDE_FROM_ALL YES)
set_property(DIRECTORY ${json_SOURCE_DIR} PROPERTY EXCLUDE_FROM_ALL YES)
set_property(DIRECTORY ${magic_enum_SOURCE_DIR} PROPERTY EXCLUDE_FROM_ALL YES)

set_target_properties(fmt PROPERTIES FOLDER "Libraries")

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
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

function(generate_filetree root files)
  foreach(source IN LISTS files)
      get_filename_component(source_path "${source}" PATH)
      string(REPLACE "/" "\\" source_path_msvc "${source_path}")
      source_group("${source_path_msvc}" FILES "${root}\\${source}")
  endforeach()
endfunction()

function(generate_blender_version build_version)
  message(STATUS "Setting up Blender version ${build_version}")

  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
  #
  # - Version Processing -
  #
  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
  set_blender_version(${build_version})

  set(BLENDER_DOWNLOAD "https://github.com/blender/blender/archive/refs/tags/v${build_version}.zip")
  set(PYTHON_WINDOWS_DEV_DOWNLOAD "https://www.nuget.org/api/v2/package/python/${PYTHON_VERSION}")
  set(PYTHON_WINDOWS_BIN_DOWNLOAD "https://www.python.org/ftp/python/${PYTHON_VERSION}/python-${PYTHON_VERSION}-embed-amd64.zip")

  string(REPLACE "." ";" PYTHON_VERSION_LIST ${PYTHON_VERSION})
  list(GET PYTHON_VERSION_LIST 0 PYTHON_MAJOR_VERSION)
  list(GET PYTHON_VERSION_LIST 1 PYTHON_MINOR_VERSION)
  list(GET PYTHON_VERSION_LIST 2 PYTHON_MICRO_VERSION)

  string(REPLACE "." ";" BLENDER_VERSION_LIST ${build_version})

  list(GET BLENDER_VERSION_LIST 0 BLENDER_MAJOR_VERSION)
  list(GET BLENDER_VERSION_LIST 1 BLENDER_MINOR_VERSION)
  string(REGEX REPLACE "[a-zA-Z]" "" BLENDER_MINOR_VERSION_SHORT ${BLENDER_MINOR_VERSION})

  set(BLENDER_ID blender-${build_version})
  set(PYTHON_ID python-${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}.${PYTHON_MICRO_VERSION})
  set(BLENDER_BIN_DIR ${BLENDER_MAJOR_VERSION}.${BLENDER_MINOR_VERSION_SHORT})

  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
  #
  # - Python -
  #
  #   Grab the python version matching the current blender
  #   so cython generates the right version.
  #
  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
  if(WIN32)
    set(BLENDER_BIN_ID blender-${build_version}-bin)
    FetchContent_Declare(
      ${BLENDER_BIN_ID}
      URL ${BLENDER_WINDOWS_BIN}
    )
    if(NOT ${BLENDER_VERSION}-bin_POPULATED)
      message(STATUS "    Installing Blender binaries")
      FetchContent_Populate(${BLENDER_BIN_ID})
    endif()
    set(BLENDER_BIN ${${BLENDER_BIN_ID}_SOURCE_DIR}/blender.exe)

    set(PYTHON_PATH ${${BLENDER_BIN_ID}_SOURCE_DIR}/${BLENDER_VERSION_SHORT}/python)
    set(PYTHON_BIN ${PYTHON_PATH}/bin/python.exe)
    string(REPLACE "\\" "/" ESCAPED_PYTHON_BIN ${PYTHON_BIN})
    set(ESCAPED_PYTHON_BIN ${ESCAPED_PYTHON_BIN} PARENT_SCOPE)

    file(DOWNLOAD
      "${PIP_DOWNLOAD}"
      "${PYTHON_PATH}/get-pip.py"
    )

    if(EXISTS ${PYTHON_PATH}/Lib/site-packages/pip)
    else()
      message(STATUS "    Installing pip")
      execute_process(COMMAND ${PYTHON_BIN} ${PYTHON_PATH}/get-pip.py)
    endif()
    file(WRITE ${PYTHON_PATH}/python${PYTHON_MAJOR_VERSION}${PYTHON_MINOR_VERSION}._pth "python39.zip\n.\nLib\\site-packages")
    if(EXISTS ${PYTHON_PATH}/Lib/site-packages/Cython)
    else()
      message(STATUS "    Installing cython")
      execute_process(COMMAND ${PYTHON_PATH}/python.exe -m pip install cython)
    endif()

    set(PYTHON_SOURCE ${PYTHON_ID}_dev)
    FetchContent_Declare(
      ${PYTHON_SOURCE}
      URL ${PYTHON_WINDOWS_DEV_DOWNLOAD}
    )
    FetchContent_GetProperties(${PYTHON_ID}_dev)
    if(NOT ${PYTHON_SOURCE}_POPULATED)
      message(STATUS "    Installing python development files")
      FetchContent_Populate(${PYTHON_SOURCE})
    endif()
    file(COPY ${${PYTHON_SOURCE}_SOURCE_DIR}/tools/libs DESTINATION ${PYTHON_PATH})
    file(COPY ${${PYTHON_SOURCE}_SOURCE_DIR}/tools/include DESTINATION ${PYTHON_PATH})
  endif()

  if(UNIX AND NOT APPLE)
    # use the python blender ships with
    set(BLENDER_BIN_ID blender-${build_version}-bin)
    FetchContent_Declare(
      ${BLENDER_BIN_ID}
      URL ${BLENDER_LINUX_BIN}
    )
    FetchContent_GetProperties(${BLENDER_BIN_ID})
    if(NOT ${BLENDER_BIN_ID}_POPULATED)
      message(STATUS "    Installing python (via blender)")
      FetchContent_Populate(${BLENDER_BIN_ID})
    endif()
    set(BLENDER_BIN ${BLENDER_BIN_ID}/blender)
    set(PYTHON_BASE_PATH "${${BLENDER_BIN_ID}_SOURCE_DIR}/${BLENDER_BIN_DIR}/python")
    set(PYTHON_PATH "${PYTHON_BASE_PATH}/bin")
    set(PYTHON_BIN "${PYTHON_PATH}/python${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}")
    file(DOWNLOAD
      "${PIP_DOWNLOAD}"
      "${PYTHON_PATH}/get-pip.py"
    )

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
    FetchContent_GetProperties(${PYTHON_SOURCE})
    if(NOT ${PYTHON_SOURCE}_POPULATED)
      message(STATUS "    Installing python development files")
      FetchContent_Populate(${PYTHON_SOURCE})
    endif()
  endif()

  set(Python_ROOT_DIR ${${PYTHON_SOURCE}_SOURCE_DIR}/tools)
  find_package(Python COMPONENTS Development)

  if (WIN32)
    execute_process(
      COMMAND
        ${PYTHON_BIN}
        ${CMAKE_CURRENT_SOURCE_DIR}/bxx/cmake/fix_python_lib_path.py
        ${Python_LIBRARIES}
      OUTPUT_VARIABLE EFFECTIVE_PYTHON_LIB
    )

    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/bxx/third_party/frame_msvc.ipp
      ${boost_SOURCE_DIR}/boost/stacktrace/detail/frame_msvc.ipp
    )
  endif()

  execute_process(
    COMMAND
      ${PYTHON_BIN}
      ${CMAKE_CURRENT_SOURCE_DIR}/bxx/cmake/generate_scripts.py
  )


  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
  #
  # - generators -
  #
  #   todo: currently, this runs with multiple python versions
  #         multiple times for the same project in multi-version
  #         builds. This shouldn't matter too much, but it's ugly
  #
  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
  set(generate_scripts generate_scripts-${build_version})
  add_custom_target(
    ${generate_scripts}
    ALL ${PYTHON_BIN} ${CMAKE_CURRENT_SOURCE_DIR}/bxx/cmake/generate_scripts.py
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCES
      ${CMAKE_CURRENT_SOURCE_DIR}/bxx/cmake/generate_scripts.py
  )
  set_target_properties(${generate_scripts} PROPERTIES FOLDER "Generators")

  # avoid calling any blender cmake scripts
  FetchContent_Declare(
    ${BLENDER_ID}
    URL ${BLENDER_DOWNLOAD}
  )
  FetchContent_GetProperties(blender)
  if(NOT ${BLENDER_ID}_POPULATED)
    message(STATUS "    Instaling blender sources")
    FetchContent_Populate(${BLENDER_ID})
  endif()

  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
  #
  # - core build library-
  #
  #   This project is used to build the cython module loaded
  #   directly by python.
  #
  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
  set(core core-${build_version})
  add_custom_target(
    ${core}
    ALL
    COMMAND
      ${PYTHON_BIN} setup.py build_ext --inplace includes="${fmt_SOURCE_DIR}/include|${${PYTHON_SOURCE}_SOURCE_DIR}/Include|${boost_SOURCE_DIR}" 
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}/bxx/core
    SOURCES
      bxx/core/core_defines.hpp
      bxx/core/core_cxx.cpp
      bxx/core/core_cy.pyx
      bxx/core/core.py
      bxx/core/setup.py
      bxx/core/util.py
      bxx/core/test_runner.py
  )
  set_target_properties(${core} PROPERTIES FOLDER ${build_version})

  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
  #
  # - bxx library -
  #
  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
  file(GLOB_RECURSE bxx_files bxx/bxx/* bxx/common/*)
  set(bxx bxx-${build_version})

  add_library(
    ${bxx} STATIC
    ${bxx_files}
    ${CMAKE_CURRENT_SOURCE_DIR}/bxx/bxx/enums.generated.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/bxx/bxx/enums.generated.cpp
  )

  set_target_properties(${bxx} PROPERTIES FOLDER ${build_version})

  target_include_directories(
    ${bxx} PRIVATE
    ${Python_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/bxx
    ${boost_SOURCE_DIR}
    ${${BLENDER_ID}_SOURCE_DIR}/source/blender
    ${${BLENDER_ID}_SOURCE_DIR}/source/blender/blenlib
    ${${BLENDER_ID}_SOURCE_DIR}/source/blender/makesdna
  )

  target_link_libraries(
    ${bxx} PUBLIC
    ${EFFECTIVE_PYTHON_LIB}
    fmt::fmt-header-only
    nlohmann_json::nlohmann_json
    magic_enum
  )

  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
  #
  # - scripts -
  #
  # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

  set(scripts_core ${CMAKE_CURRENT_SOURCE_DIR}/bxx/core/lib_api.cpp)
  source_group("refs" FILES "${CMAKE_CURRENT_SOURCE_DIR}\\CMakeLists.txt" ${scripts_core})

  file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/scripts ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*)

  function(make_script_target child child_path root full)
    if(IS_DIRECTORY ${full})
      file(GLOB_RECURSE script_files ${full}/*)
      file(GLOB_RECURSE script_files_rel
        RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}/${root}
        ${full}/*
      )
      list(REMOVE_ITEM script_files_rel ${child_path}/calls.generated.cpp)
      set(OLD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
      if(EXISTS "${full}/calls.generated.cpp")
      else()
        file(TOUCH "${full}/calls.generated.cpp")
      endif()

      set(script_name "${child}-${build_version}")

      add_library(${script_name} SHARED
        ${script_files}
        ${scripts_core}
        ${full}/calls.generated.cpp
      )

      set_target_properties(${script_name} PROPERTIES FOLDER ${build_version})

      source_group("refs" FILES "${full}\\calls.generated.cpp")
      add_dependencies(${script_name} ${generate_scripts})

      add_custom_command(
        TARGET ${script_name}
        POST_BUILD
        COMMAND ${PYTHON_BIN} ${CMAKE_CURRENT_SOURCE_DIR}/bxx/cmake/rename_lib.py
          ${CMAKE_CURRENT_SOURCE_DIR}
          ${build_version}
          ${CMAKE_BUILD_TYPE}
          $<TARGET_FILE:${script_name}>
      )

      target_include_directories(
        ${script_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/bxx
        ${${BLENDER_ID}_SOURCE_DIR}/source/blender
        ${${BLENDER_ID}_SOURCE_DIR}/source/blender/blenlib
        ${${BLENDER_ID}_SOURCE_DIR}/source/blender/makesdna
        ${SCRIPT_INCLUDE_DIRECTORIES}
        ${Python_INCLUDE_DIRS}
        ${boost_SOURCE_DIR}
      )
      target_link_libraries(
        ${script_name} PRIVATE
        ${bxx}
        ${EFFECTIVE_PYTHON_LIB}
        ${SCRIPT_LIBRARIES}
        magic_enum
      )

      target_link_options(${script_name} PRIVATE /pdbaltpath:${script_name}.pdb)
      generate_filetree(${root} "${script_files_rel}")
    endif()
  endfunction()

  foreach(child ${children})
    make_script_target(${child} ${child} "scripts" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/${child}")
  endforeach()
  string(REPLACE "\\" "/" ESCAPED_BLENDER_BIN ${BLENDER_BIN})
  string(REPLACE "\\" "/" ESCAPED_BLENDER_SRC ${${BLENDER_BIN_ID}_SOURCE_DIR})

  string(REPLACE "." "_" FIXED_ADDON_NAME ${BXX_ADDON_NAME})
  string(REPLACE "-" "_" FIXED_ADDON_NAME ${BXX_ADDON_NAME})

  list(APPEND INSTALL_TEST_COMMANDS "
    message(\"Running unit tests for blender version ${BLENDER_VERSION}\")
    execute_process(COMMAND \"${ESCAPED_PYTHON_BIN}\" \"${ESCAPED_CURRENT_SOURCE_DIR}/bxx/cmake/install_version.py\" \"${ESCAPED_INSTALL_PREFIX}\" \"${ESCAPED_BLENDER_SRC}/${BLENDER_VERSION_SHORT}/scripts/addons/${BXX_ADDON_NAME}\")
    execute_process(COMMAND \"${ESCAPED_BLENDER_BIN}\" -b -P \"${ESCAPED_BLENDER_SRC}/${BLENDER_VERSION_SHORT}/scripts/addons/${BXX_ADDON_NAME}/bxx/cmake/bl_run_tests.py\" RESULT_VARIABLE BXX_TEST_RESULT)
    if(\${BXX_TEST_RESULT})
      message(SEND_ERROR \"Tests failed for blender version ${BLENDER_VERSION_SHORT}\")
    endif()
  ")
  set(INSTALL_TEST_COMMANDS ${INSTALL_TEST_COMMANDS} PARENT_SCOPE)
endfunction()

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - Install -
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

function(finish_bxx_generate)
  install(CODE "
    message(\"Installing and bundling addon ${BXX_ADDON_NAME} \")
    execute_process(COMMAND \"${ESCAPED_PYTHON_BIN}\" \"${ESCAPED_CURRENT_SOURCE_DIR}/bxx/cmake/install.py\" \"${ESCAPED_CURRENT_SOURCE_DIR}\" \"${ESCAPED_INSTALL_PREFIX}\" \"${ESCAPED_ZIP_INSTALL_PATH}\" RESULT_VARIABLE BXX_INSTALL_RESULT ERROR_VARIABLE BXX_INSTALL_ERROR)
    if(\${BXX_INSTALL_RESULT})
      message(FATAL_ERROR \"Failed to install bxx addon:\n\${BXX_INSTALL_ERROR}\")
    endif()
  ")

  foreach(command ${INSTALL_TEST_COMMANDS})
    install( CODE ${command} )
  endforeach()
endfunction()