include(FetchContent)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - Build Type -
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

set(BUILD_TYPES "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
set(BUILD_TYPE "RelWithDebInfo" CACHE STRING "")
set_property(CACHE BUILD_TYPE PROPERTY STRINGS ${BUILD_TYPES})
set(CMAKE_BUILD_TYPE ${BUILD_TYPE})

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - Blender Version -
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
set(BLENDER_VERSIONS "3.0" "2.79")
set(BLENDER_VERSION "3.0" CACHE STRING "Blender version to build for")
set_property(CACHE BLENDER_VERSION PROPERTY STRINGS ${BLENDER_VERSIONS})

if(${BLENDER_VERSION} EQUAL "3.0")
  set(BLENDER_DOWNLOAD https://github.com/blender/blender/archive/refs/tags/v3.0.0.zip)
  set(BXX_DEFINE BXX_3_0_0)
elseif(${BLENDER_VERSION} EQUAL "2.79")
  set(BLENDER_DOWNLOAD https://github.com/blender/blender/archive/refs/tags/v2.79b.zip)
  set(BXX_DEFINE BXX_2_79)
else()
  message( FATAL_ERROR "Invalid Blender version ${BLENDER_VERSION}")
endif()

add_compile_definitions(BXX_DEFINE)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - Libraries -
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
  blender
  URL ${BLENDER_DOWNLOAD}
)

FetchContent_MakeAvailable(fmt json)

# avoid calling any blender cmake scripts
FetchContent_GetProperties(blender)
if(NOT blender_POPULATED)
  FetchContent_Populate(blender)
endif()

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# - core dummy library-
#
#   just a dummy project to load/compile in IDE,
#   it won't be used by anything.
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
add_library(
  core-dummy STATIC bxx/core/core_cxx.cpp bxx/core/core.pyx
)
target_include_directories(core-dummy PRIVATE bxx/common)

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
  ${blender_SOURCE_DIR}/source/blender
  ${blender_SOURCE_DIR}/source/blender/blenlib
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
    target_include_directories(
      ${child} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/bxx/bxx
      ${CMAKE_CURRENT_SOURCE_DIR}/bxx/common
    )
    target_link_libraries(${child} PRIVATE bxx)
    generate_filetree(${script_files_rel})
  endif()
endforeach()