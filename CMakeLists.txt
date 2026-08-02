cmake_minimum_required(VERSION 3.26)
project(GameProgrammingCpp LANGUAGES CXX)

# 自分のターゲット共通の設定。依存ライブラリのビルドには掛けない。
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_23)

if(MSVC)
    target_compile_options(project_options INTERFACE /utf-8)
    target_compile_definitions(project_options INTERFACE UNICODE _UNICODE WIN32_LEAN_AND_MEAN)
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
# 本書で使う PNG / JPEG は stb_image バックエンドで読めるため、
# 外部ライブラリを丸ごとビルドする形式も落として構成時間を削る。
set(SDL2IMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_JXL OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_TIF OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_WEBP OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(sdl sdl_image)

# 章ごとの実行ファイルを定義する。
#   SOURCES     ビルド対象のソース
#   LIBS        SDL2 以外に追加でリンクするターゲット
#   ASSET_DIRS  出力ディレクトリにコピーするディレクトリ名
function(add_chapter NAME)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBS;ASSET_DIRS" ${ARGN})

    add_executable(${NAME} ${ARG_SOURCES})
    target_link_libraries(${NAME} PRIVATE
            project_options
            SDL2::SDL2
            SDL2::SDL2main
            ${ARG_LIBS})

    # Windows は RPATH を持たないので、依存 DLL を exe の隣に置く。
    # 静的ビルドではリストが空になるので、その場合はコマンドごと消えるようにしておく。
    if(WIN32)
        add_custom_command(TARGET ${NAME} POST_BUILD
                COMMAND "$<$<BOOL:$<TARGET_RUNTIME_DLLS:${NAME}>>:${CMAKE_COMMAND};-E;copy_if_different;$<TARGET_RUNTIME_DLLS:${NAME}>;$<TARGET_FILE_DIR:${NAME}>>"
                COMMAND_EXPAND_LISTS
                COMMENT "Copying runtime DLLs to output directory...")
    endif()

    # Assets は .gitignore で除外しているため、クローン直後や CI には存在しない。
    foreach(DIR IN LISTS ARG_ASSET_DIRS)
        if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}")
            message(STATUS "${NAME}: ${DIR} が無いためコピーをスキップします")
            continue()
        endif()

        add_custom_command(TARGET ${NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
                "$<TARGET_FILE_DIR:${NAME}>/${DIR}"
                COMMENT "Copying ${DIR} to output directory...")
    endforeach()
endfunction()

add_subdirectory(Chapter01)
add_subdirectory(Chapter02)
