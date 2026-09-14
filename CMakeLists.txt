cmake_minimum_required(VERSION 3.20)

project(VLTEngine
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================================================
# SDL2
# =========================================================

set(SDL2_ROOT "C:/msys64/ucrt64")

set(SDL2_INCLUDE_DIR
    "${SDL2_ROOT}/include/SDL2"
)

set(SDL2_LIBRARY
    "${SDL2_ROOT}/lib/libSDL2.dll.a"
)

set(GLM_ROOT "C:/msys64/ucrt64/include")

# =========================================================
# GLAD
# =========================================================

set(GLAD_ROOT
    "${CMAKE_CURRENT_SOURCE_DIR}/External/GLAD"
)

add_library(GLAD STATIC
    "${GLAD_ROOT}/src/glad.c"
)

target_include_directories(GLAD
    PUBLIC
        "${GLAD_ROOT}/include"
)

# =========================================================
# VLTEngine Source Files
# =========================================================

file(GLOB_RECURSE VLTEngine_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# =========================================================
# VLTEngine Executable
# =========================================================

add_executable(VLTEngine
    ${VLTEngine_SOURCES}
)

# =========================================================
# Include Directories
# =========================================================

target_include_directories(VLTEngine
    PRIVATE
        ${SDL2_INCLUDE_DIR}
        ${GLM_ROOT}
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine
)

# =========================================================
# Libraries
# =========================================================

target_link_libraries(VLTEngine
    PRIVATE
        ${SDL2_LIBRARY}
        GLAD
        opengl32
)

# =========================================================
# Windows
# =========================================================

if(WIN32)
    target_compile_definitions(VLTEngine
        PRIVATE
            SDL_MAIN_HANDLED
    )
endif()