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

# SDL2_ttf。第 11 章の UI で、文字列を描画してテクスチャに焼くのに使う。
# HarfBuzz（複雑な字形整形）は本書では使わないので、サブモジュールごと取得しない。
FetchContent_Declare(
        sdl_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
        GIT_TAG release-2.24.0
        GIT_PROGRESS TRUE
        GIT_SUBMODULES "external/freetype"
)

# SDL2_ttf は FreeType を必要とする。SDL2TTF_VENDORED の既定値は MSVC では ON、
# それ以外では OFF で、OFF だとシステムの FreeType を探しに行く。CI のランナーには
# 開発パッケージが入っていないため、常に同梱版をビルドさせて 3 OS で揃える。
set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDL2TTF_HARFBUZZ OFF CACHE BOOL "" FORCE)
set(SDL2TTF_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL2TTF_INSTALL OFF CACHE BOOL "" FORCE)

# GLEW は他の 2 つと違い git リポジトリからは取得できない。
#   - リポジトリには生成済みの include/GL/glew.h と src/glew.c が入っていない
#     （本来 `make extensions` が OpenGL レジストリから生成する）。リリース版の
#     アーカイブには生成済みのものが同梱されているので、そちらを使う。
#   - CMake プロジェクトがリポジトリ直下ではなく build/cmake にあるため
#     SOURCE_SUBDIR で位置を教える必要がある。
FetchContent_Declare(
        glew
        URL https://github.com/nigels-com/glew/releases/download/glew-2.3.1/glew-2.3.1.zip
        URL_HASH SHA256=09e0083ae46930aba9b53e72c92ee1a557e24ed393526fec26cb0ebabd834720
        SOURCE_SUBDIR build/cmake
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# glewinfo / visualinfo は本書では使わない。
set(BUILD_UTILS OFF CACHE BOOL "" FORCE)

# SOIL（Simple OpenGL Image Library）。本書が OpenGL 章のテクスチャ読み込みに使う。
# タグが打たれていないリポジトリなのでコミットで固定する。
FetchContent_Declare(
        soil
        GIT_REPOSITORY https://github.com/kbranigan/Simple-OpenGL-Image-Library
        GIT_TAG 7a056e19c3b272e42b5e0df8bb1cf4f470434b3e
        GIT_PROGRESS TRUE
)

# RapidJSON。本書は第 6 章の .gpmesh（JSON）の読み込みに使う。
# ヘッダーオンリーだが付属の CMakeLists はテスト・サンプル・ドキュメントのビルドや
# GTest / Doxygen の探索まで行うので、add_subdirectory はせずヘッダーだけ取得する。
# SOURCE_SUBDIR に存在しないディレクトリを指定すると、FetchContent_MakeAvailable は
# 取得だけ行って add_subdirectory を省略する。
# タグは v1.1.0（2016 年）が最後で C++17 以降の対応が入っていないため、master の
# コミットで固定する。
FetchContent_Declare(
        rapidjson
        GIT_REPOSITORY https://github.com/Tencent/rapidjson
        GIT_TAG 24b5e7a8b27f42fa16b96fc70aade9106cf7102f
        GIT_PROGRESS TRUE
        SOURCE_SUBDIR do-not-add-subdirectory
)

# SOIL の CMakeLists は cmake_minimum_required(VERSION 2.6) のままで、
# CMake 4 は 3.5 未満との互換を削除したためそのままではエラーになる。
# 下限を引き上げて古い指定を上書きする（3.5 だと今度は非推奨警告が出るので 3.10）。
# SDL / glew は 3.16 以上を要求しているのでこの値の影響を受けない。
set(CMAKE_POLICY_VERSION_MINIMUM 3.10)

# AVIF は vendored dav1d のビルドに NASM を要求するので無効化する。
# 本書で使う PNG / JPEG は stb_image バックエンドで読めるため、
# 外部ライブラリを丸ごとビルドする形式も落として構成時間を削る。
set(SDL2IMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_JXL OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_TIF OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_WEBP OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(sdl sdl_image sdl_ttf glew rapidjson)

# SOIL の add_library は STATIC / SHARED を書いていないので BUILD_SHARED_LIBS に従う。
# SDL がキャッシュに BUILD_SHARED_LIBS=ON を残すため、放っておくと SOIL.dll になる。
# SOIL は dllexport を一切書いていない 2007 年のコードなのでエクスポートが空になり、
# インポートライブラリ SOIL.lib が生成されず LNK1104 でリンクに失敗する。
# 静的ライブラリとして作らせる（SDL / glew の共有ビルドには影響させない）。
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(soil)
unset(BUILD_SHARED_LIBS)

# glew の CMakeLists は include ディレクトリを include_directories()（ディレクトリ
# スコープ）でしか設定しておらず、利用側のターゲットには伝わらない。
# ビルドツリー用の usage requirement を補う。
# （target_include_directories は $<INSTALL_INTERFACE:...> しか設定していない）
# glew は install(EXPORT) の対象なので、ビルドツリーの絶対パスをそのまま入れると
# 「build ディレクトリ配下のパスが混ざっている」と怒られる。BUILD_INTERFACE で包む。
#
# glew.h は既定で GLU（<GL/glu.h>）を巻き込む。GLU は Linux では別パッケージ
# （libglu1-mesa-dev）で、入っていないとインクルードエラーになる。本書は GLU を
# 使わないので取り込みを止める。glew 自身のビルドと利用側の両方に効かせる。
#
# glew の CMakeLists は共有版 glew と静的版 glew_s の両方を常に作るので、
# どちらを使うことになっても良いように両方へ設定する。
foreach(GLEW_TARGET glew glew_s)
    target_include_directories(${GLEW_TARGET} PUBLIC "$<BUILD_INTERFACE:${glew_SOURCE_DIR}/include>")
    target_compile_definitions(${GLEW_TARGET} PUBLIC GLEW_NO_GLU)
endforeach()

# 静的版（glew_s）を採用する。共有版だと glew32d.dll が exe の隣に置かれるが、
# Windows の Smart App Control はローカルビルドの DLL をファイル単位で評価し、
# 再ビルドで内容が変わるたびに未判定の新しいファイルとしてブロックすることがある
# （STATUS_SYSTEM_INTEGRITY_POLICY_VIOLATION = 0xC0E90002 で即終了する）。
# 静的リンクなら配布物が減り、この判定自体が発生しない。
# glew_s は INTERFACE で GLEW_STATIC を定義するので、利用側の宣言も自動で合う。
if(NOT TARGET GLEW::GLEW)
    add_library(GLEW::GLEW ALIAS glew_s)
endif()

# SOIL も glew と同様にインクルードディレクトリを利用側へ伝えていない。
# ヘッダーは src/SOIL.h にあるが、本書は <SOIL/SOIL.h> の形で include するので、
# install 後と同じ SOIL/ 階層をビルドツリーに用意して両方の書き方を通す。
file(COPY "${soil_SOURCE_DIR}/src/SOIL.h" DESTINATION "${soil_BINARY_DIR}/include/SOIL")
target_include_directories(SOIL PUBLIC
        "$<BUILD_INTERFACE:${soil_SOURCE_DIR}/src>"
        "$<BUILD_INTERFACE:${soil_BINARY_DIR}/include>")

# SOIL は 2007 年の C コード。fopen などで MSVC の C4996 が大量に出るので黙らせる。
if(MSVC)
    target_compile_definitions(SOIL PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# macOS では SOIL が CFBundle 経由で OpenGL 拡張のアドレスを引く（SOIL.c の
# query_DXT_capability）。SOIL 側は OpenGL しかリンクしていないため、
# CoreFoundation のシンボルが未解決になる。
# SOIL の CMakeLists は plain 形式の target_link_libraries を使っているので、
# こちらも PUBLIC などのキーワードを付けずに揃える（混在はエラーになる）。
if(APPLE)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation REQUIRED)
    target_link_libraries(SOIL ${COREFOUNDATION_LIBRARY})
endif()

# 名前空間付きの別名を用意する。綴りを間違えたときに configure 時点で
# 「そんなターゲットは無い」と検出されるため（:: を含む名前だけの挙動）。
if(NOT TARGET SOIL::SOIL)
    add_library(SOIL::SOIL ALIAS SOIL)
endif()

# RapidJSON は add_subdirectory していないのでターゲットが無い。
# ヘッダーの位置だけを伝える INTERFACE ライブラリを自前で用意する。
# SYSTEM を付けて、RapidJSON 側のヘッダーが出す警告を自分のビルドに持ち込まない。
add_library(rapidjson INTERFACE)
target_include_directories(rapidjson SYSTEM INTERFACE "${rapidjson_SOURCE_DIR}/include")

if(NOT TARGET RapidJSON::RapidJSON)
    add_library(RapidJSON::RapidJSON ALIAS rapidjson)
endif()

# FMOD Studio API。第 7 章のオーディオで使う。
# 再配布できないライブラリなので FetchContent では取得できず、各自がインストール
# したものを cmake/FindFMOD.cmake で探す。見つからなくてもエラーにはしない
# （CI のランナーには存在しないため）。Chapter07 側が FMOD_FOUND を見て
# 自分をビルド対象に加えるかどうかを決める。
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(FMOD)

# 章ごとの実行ファイルを定義する。
#   SOURCES       ビルド対象のソース
#   FMOD_SOURCES  FMOD のヘッダーを必要とするソース
#   LIBS          SDL2 以外に追加でリンクするターゲット
#   FMOD_LIBS     FMOD が見つかったときだけ追加でリンクするターゲット
#   ASSET_DIRS    出力ディレクトリにコピーするディレクトリ名
#
# FMOD は再配布できないため CI のランナーには存在しない。FMOD_SOURCES を指定した章は、
# FMOD が無い環境では「実行ファイルを作らず、FMOD 非依存のソースだけをコンパイルする」
# オブジェクトライブラリになる。リンクはできなくても、ヘッダーが自分の使う型を自分で
# include しているか（GCC / Clang で通るか）の検査は 3 OS で続けられる。
# 第 6 章まではこの検査で <cstdint> や <algorithm> の漏れを実際に拾っている。
function(add_chapter NAME)
    cmake_parse_arguments(ARG "" "" "SOURCES;FMOD_SOURCES;LIBS;FMOD_LIBS;ASSET_DIRS" ${ARGN})

    if(ARG_FMOD_SOURCES AND NOT FMOD_FOUND)
        list(LENGTH ARG_SOURCES _compiled)
        list(LENGTH ARG_FMOD_SOURCES _skipped)
        message(STATUS
                "${NAME}: FMOD が無いため実行ファイルは作りません"
                "（${_compiled} ファイルをコンパイル、${ARG_FMOD_SOURCES} を除外）")

        # OBJECT ライブラリはコンパイルするだけでリンクしない。
        # target_link_libraries はインクルードパスと定義を貰うためだけに使う。
        add_library(${NAME} OBJECT ${ARG_SOURCES})
        target_link_libraries(${NAME} PRIVATE
                project_options
                SDL2::SDL2
                ${ARG_LIBS})

        return()
    endif()

    add_executable(${NAME} ${ARG_SOURCES} ${ARG_FMOD_SOURCES})
    target_link_libraries(${NAME} PRIVATE
            project_options
            SDL2::SDL2
            SDL2::SDL2main
            ${ARG_LIBS}
            ${ARG_FMOD_LIBS})

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
add_subdirectory(Chapter03)
add_subdirectory(Chapter04)
add_subdirectory(Chapter05)
add_subdirectory(Chapter06)
add_subdirectory(Chapter07)
add_subdirectory(Chapter08)
add_subdirectory(Chapter09)
add_subdirectory(Chapter10)
add_subdirectory(Chapter11)
