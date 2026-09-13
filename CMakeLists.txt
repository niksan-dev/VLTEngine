cmake_minimum_required(VERSION 3.20)

project(VLTEngine
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SDL2_ROOT "C:/msys64/ucrt64")

set(SDL2_INCLUDE_DIR
    "${SDL2_ROOT}/include/SDL2"
)

set(SDL2_LIBRARY
    "${SDL2_ROOT}/lib/libSDL2.dll.a"
)

add_executable(VLTEngine
    src/main.cpp

    Engine/Core/Engine.cpp
)

target_include_directories(VLTEngine
    PRIVATE
        ${SDL2_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine
)

target_link_libraries(VLTEngine
    PRIVATE
        ${SDL2_LIBRARY}
)

if(WIN32)
    target_compile_definitions(VLTEngine
        PRIVATE
            SDL_MAIN_HANDLED
    )
endif()