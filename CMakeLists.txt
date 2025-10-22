cmake_minimum_required(VERSION 3.15...3.30)

# Proyecto en C++
project(SIM_3_GDV VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Checked;Profile" CACHE STRING "" FORCE)

# Selecciona la versión C++20 del estándar (sin extensiones)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Código fuente de la aplicación, añadirlos como si la raiz estuviera ya en /src 

set(MY_SOURCE_FILES 
    "main.cpp" 
    "callbacks.cpp"
    "RenderUtils.cpp"

    "Particle.cpp"
    "ParticleSystem.cpp"
    "ParticleGenerator.cpp"
    "UniformParticleGenerator.cpp"
    "GaussianParticleGenerator.cpp"
    "Policies.cpp"
    
    "Render/Camera.cpp"
    "Render/Render.cpp"
)

# Include directories

set(MY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/skeleton)

set(MY_INCLUDE_DIRECTORIES
    "${MY_SOURCE_DIR}"
    "${MY_SOURCE_DIR}/Physics"
    "${MY_SOURCE_DIR}/Render"
    "${MY_SOURCE_DIR}/Utils")

set(APPENDED_SOURCE_FILES "")

foreach(file IN LISTS MY_SOURCE_FILES)
    list(APPEND APPENDED_SOURCE_FILES "${MY_SOURCE_DIR}/${file}")
endforeach()

# Asignacion de ficheros fuente propios

add_executable(game ${APPENDED_SOURCE_FILES})
target_include_directories(game PRIVATE ${MY_INCLUDE_DIRECTORIES})

# A partir de aquí empiezan las dependencias

set(INCLUDE_DEPENDENCIES "")
set(LIB_DEPENDENCIES "")

function(add_multiconfig_lib base_path target_name lib_base_name lib_suffix)
    add_library(${target_name} STATIC IMPORTED)
    set_target_properties(${target_name} PROPERTIES
        IMPORTED_LOCATION_RELEASE  "${base_path}/${lib_base_name}${lib_suffix}.lib"
        IMPORTED_LOCATION_DEBUG    "${base_path}/${lib_base_name}DEBUG${lib_suffix}.lib"
        IMPORTED_LOCATION_CHECKED  "${base_path}/${lib_base_name}CHECKED${lib_suffix}.lib"
        IMPORTED_LOCATION_PROFILE  "${base_path}/${lib_base_name}PROFILE${lib_suffix}.lib"
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

target_link_libraries(game
    PRIVATE ${LIB_DEPENDENCIES})

target_include_directories(game PRIVATE
    "${INCLUDE_DEPENDENCIES}")

set_target_properties(game PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin
    OUTPUT_NAME_DEBUG game_Debug
    OUTPUT_NAME_RELEASE game_Release
)