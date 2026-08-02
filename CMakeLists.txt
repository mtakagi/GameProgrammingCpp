cmake_minimum_required(VERSION 3.20)
project(GameProgramingC++ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(UNICODE _UNICODE WIN32_LEAN_AND_MEAN)
endif()

include(FetchContent)
FetchContent_Declare(
        sdl
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG release-2.32.10
        GIT_PROGRESS TRUE
)

FetchContent_Declare(
        sdl_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
        GIT_TAG release-2.8.12
        GIT_PROGRESS TRUE
)

# AVIF は vendored dav1d のビルドに NASM を要求するので無効化する。
# 本書で使う PNG / JPEG は stb_image バックエンドで読める。
set(SDL2IMAGE_AVIF OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(sdl sdl_image)

add_subdirectory(Chapter01)
add_subdirectory(Chapter02)