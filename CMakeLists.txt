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
  GIT_PROGRESS TRUE)

FetchContent_MakeAvailable(sdl)

add_subdirectory(Chapter01)
add_subdirectory(Chapter02)