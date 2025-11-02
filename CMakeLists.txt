cmake_minimum_required(VERSION 3.15...3.30)

# Proyecto en C++
project(SIM_3_GDV VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Checked;Profile;RelWithDebInfo" CACHE STRING "" FORCE)

# Selecciona la versión C++17 del estándar (sin extensiones)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Suppress CMP0175 warning (if needed)
if(POLICY CMP0175)
    cmake_policy(SET CMP0175 OLD)
endif()

# Root paths
set(SKELETON_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/skeleton/src")
set(SKELETON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/skeleton/include")

# Collect source files recursively
file(GLOB_RECURSE MY_SOURCE_FILES CONFIGURE_DEPENDS 
    "${SKELETON_SRC_DIR}/*.cpp"
)

# # Explicitly add MeshData.cpp
# list(APPEND MY_SOURCE_FILES "${SKELETON_SRC_DIR}/utils/MeshData.cpp")

# Include directories
set(MY_INCLUDE_DIRECTORIES
    "${SKELETON_INCLUDE_DIR}"

    "${SKELETON_INCLUDE_DIR}/render"

    "${SKELETON_INCLUDE_DIR}/particle"
    "${SKELETON_INCLUDE_DIR}/particle/generators"
    "${SKELETON_INCLUDE_DIR}/particle/systems"
    "${SKELETON_INCLUDE_DIR}/particle/systems/scenery"
    "${SKELETON_INCLUDE_DIR}/particle/types"
    "${SKELETON_INCLUDE_DIR}/particle/policies"

    "${SKELETON_INCLUDE_DIR}/force"
    "${SKELETON_INCLUDE_DIR}/force/generators"
    "${SKELETON_INCLUDE_DIR}/force/types"
    "${SKELETON_INCLUDE_DIR}/force/types/regional"

    "${SKELETON_INCLUDE_DIR}/utils"
)

add_executable(game ${MY_SOURCE_FILES})

# Apply project-specific include directories
target_include_directories(game PRIVATE ${MY_INCLUDE_DIRECTORIES})

set(INCLUDE_DEPENDENCIES "")
set(LIB_DEPENDENCIES "")

function(add_multiconfig_lib base_path target_name lib_base_name lib_suffix)
    add_library(${target_name} STATIC IMPORTED)
    set_target_properties(${target_name} PROPERTIES
        IMPORTED_LOCATION_RELEASE  "${base_path}/${lib_base_name}${lib_suffix}.lib"
        IMPORTED_LOCATION_DEBUG    "${base_path}/${lib_base_name}DEBUG${lib_suffix}.lib"
        IMPORTED_LOCATION_CHECKED  "${base_path}/${lib_base_name}CHECKED${lib_suffix}.lib"
        IMPORTED_LOCATION_PROFILE  "${base_path}/${lib_base_name}PROFILE${lib_suffix}.lib"
        IMPORTED_LOCATION_RELWITHDEBINFO "${base_path}/${lib_base_name}${lib_suffix}.lib"
    )
endfunction()

# ---------------------------------------------------------------------
set(COMMON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/common/Common")
list(APPEND INCLUDE_DEPENDENCIES ${COMMON_PATH})

# ---------------------------------------------------------------------
set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/common/lib/vc14win64")
add_multiconfig_lib("${LIB_PATH}" "UtilsLib" "Utils" "")
list(APPEND LIB_DEPENDENCIES UtilsLib)

# ---------------------------------------------------------------------
set(GRAPHICS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/common/Graphics")

# Ver arquitectura
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(GLUT_LIB "${GRAPHICS_PATH}/lib/win64/glut/glut32.lib")
else()
    set(GLUT_LIB "${GRAPHICS_PATH}/lib/win32/glut/glut32.lib")
endif()

list(APPEND LIB_DEPENDENCIES ${GLUT_LIB})
list(APPEND INCLUDE_DEPENDENCIES ${GRAPHICS_PATH}/include/win32/GL)

# ---------------------------------------------------------------------
set(PHYSX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/common/PhysX-3.4")

set(PHYSX_CORE_PATH "${PHYSX_PATH}/PhysX_3.4")
set(PHYSX_CORE_LIB_PATH "${PHYSX_CORE_PATH}/Lib/vc14win64")

add_multiconfig_lib("${PHYSX_CORE_LIB_PATH}" "PhysX3" "PhysX3" "_x64")
add_multiconfig_lib("${PHYSX_CORE_LIB_PATH}" "PhysX3CharacterKinematic" "PhysX3CharacterKinematic" "_x64")
add_multiconfig_lib("${PHYSX_CORE_LIB_PATH}" "PhysX3Common" "PhysX3Common" "_x64")
add_multiconfig_lib("${PHYSX_CORE_LIB_PATH}" "PhysX3Cooking" "PhysX3Cooking" "_x64")
add_multiconfig_lib("${PHYSX_CORE_LIB_PATH}" "PhysX3Extensions" "PhysX3Extensions" "")

list(APPEND LIB_DEPENDENCIES 
    "PhysX3"
    "PhysX3CharacterKinematic"
    "PhysX3Common"
    "PhysX3Cooking"
    "PhysX3Extensions")

list(APPEND INCLUDE_DEPENDENCIES "${PHYSX_CORE_PATH}/Include")
    
set(PHYSX_SHARED_PATH "${PHYSX_PATH}/PxShared")
set(PHYSX_SHARED_LIB_PATH "${PHYSX_SHARED_PATH}/lib/vc14win64")

add_multiconfig_lib("${PHYSX_SHARED_LIB_PATH}" "PsFastXml" "PsFastXml" "_x64")
add_multiconfig_lib("${PHYSX_SHARED_LIB_PATH}" "PxFoundation" "PxFoundation" "_x64")
add_multiconfig_lib("${PHYSX_SHARED_LIB_PATH}" "PxPvdSDK" "PxPvdSDK" "_x64")
add_multiconfig_lib("${PHYSX_SHARED_LIB_PATH}" "PxTask" "PxTask" "_x64")

list(APPEND LIB_DEPENDENCIES 
    "PsFastXml"
    "PxFoundation"
    "PxPvdSDK"
    "PxTask")

list(APPEND INCLUDE_DEPENDENCIES "${PHYSX_SHARED_PATH}/include")

# ---------------------------------------------------------------------
set(UTILS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/common/Utils")
list(APPEND INCLUDE_DEPENDENCIES "${PHYSX_SHARED_PATH}/include")

# ---------------------------------------------------------------------
# Fetch and Configure Assimp (moved to after INCLUDE_DEPENDENCIES initialization)
include(FetchContent)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.2
)

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Build Assimp tools" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build Assimp tests" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Install Assimp" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Build Zlib with Assimp" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Build Assimp samples" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static Assimp library" FORCE)

FetchContent_MakeAvailable(assimp)

list(APPEND LIB_DEPENDENCIES assimp::assimp)
list(APPEND INCLUDE_DEPENDENCIES ${assimp_SOURCE_DIR}/include/assimp ${assimp_BINARY_DIR}/include)

# ---------------------------------------------------------------------
target_link_libraries(game PRIVATE ${LIB_DEPENDENCIES})
target_include_directories(game PRIVATE ${INCLUDE_DEPENDENCIES})

set_target_properties(game PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin
    OUTPUT_NAME_DEBUG game_Debug
    OUTPUT_NAME_RELEASE game_Release
)