# FMOD Studio API を探し、FMOD::Core / FMOD::Studio を定義する。
#
# FMOD はプロプライエタリで再配布できないため、SDL や GLEW のように FetchContent で
# 取得することができない。各自がインストールしたものを探す方式になる。
# 見つからない場合は FMOD_FOUND が偽になるだけでエラーにはしない。
# CI のランナーには FMOD が無いので、呼び出し側（Chapter07）はこの値を見て
# 自分をビルド対象から外す。
#
# 探索場所を明示したい場合:
#   cmake -DFMOD_ROOT="D:/SDK/FMOD Studio API Windows" ...
#   もしくは環境変数 FMOD_ROOT

set(_fmod_hints "")

if(FMOD_ROOT)
    list(APPEND _fmod_hints "${FMOD_ROOT}")
endif()

if(DEFINED ENV{FMOD_ROOT})
    list(APPEND _fmod_hints "$ENV{FMOD_ROOT}")
endif()

if(WIN32)
    list(APPEND _fmod_hints
            "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows"
            "C:/Program Files/FMOD SoundSystem/FMOD Studio API Windows")
elseif(APPLE)
    list(APPEND _fmod_hints "/Applications/FMOD Studio API Macintosh")
else()
    list(APPEND _fmod_hints
            "$ENV{HOME}/FMOD Studio API Linux"
            "/opt/fmodstudioapi")
endif()

# ライブラリはアーキテクチャごとにサブディレクトリが分かれている。
# macOS だけはユニバーサルバイナリが lib 直下に 1 本置かれる。
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(_fmod_libdir "lib/x86")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "[Aa][Rr][Mm]64")
        set(_fmod_libdir "lib/arm64")
    else()
        set(_fmod_libdir "lib/x64")
    endif()
elseif(APPLE)
    set(_fmod_libdir "lib")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "[Aa]arch64|[Aa][Rr][Mm]64")
    set(_fmod_libdir "lib/arm64")
else()
    set(_fmod_libdir "lib/x86_64")
endif()

find_path(FMOD_CORE_INCLUDE_DIR
        NAMES fmod.hpp
        HINTS ${_fmod_hints}
        PATH_SUFFIXES api/core/inc)

find_path(FMOD_STUDIO_INCLUDE_DIR
        NAMES fmod_studio.hpp
        HINTS ${_fmod_hints}
        PATH_SUFFIXES api/studio/inc)

# Windows のインポートライブラリは *_vc.lib、他プラットフォームは lib*.so / lib*.dylib。
# 末尾が L のものはログ出力付きのビルドで、Debug 構成ではこちらを使う。
find_library(FMOD_CORE_LIBRARY
        NAMES fmod_vc fmod
        HINTS ${_fmod_hints}
        PATH_SUFFIXES "api/core/${_fmod_libdir}")

find_library(FMOD_CORE_LIBRARY_DEBUG
        NAMES fmodL_vc fmodL
        HINTS ${_fmod_hints}
        PATH_SUFFIXES "api/core/${_fmod_libdir}")

find_library(FMOD_STUDIO_LIBRARY
        NAMES fmodstudio_vc fmodstudio
        HINTS ${_fmod_hints}
        PATH_SUFFIXES "api/studio/${_fmod_libdir}")

find_library(FMOD_STUDIO_LIBRARY_DEBUG
        NAMES fmodstudioL_vc fmodstudioL
        HINTS ${_fmod_hints}
        PATH_SUFFIXES "api/studio/${_fmod_libdir}")

# Windows では .lib はインポートライブラリでしかないので、実体の DLL も要る。
# IMPORTED_LOCATION に DLL を入れておくと add_chapter の $<TARGET_RUNTIME_DLLS> が
# exe の隣へ自動でコピーしてくれる。
if(WIN32)
    find_file(FMOD_CORE_DLL          NAMES fmod.dll         HINTS ${_fmod_hints} PATH_SUFFIXES "api/core/${_fmod_libdir}")
    find_file(FMOD_CORE_DLL_DEBUG    NAMES fmodL.dll        HINTS ${_fmod_hints} PATH_SUFFIXES "api/core/${_fmod_libdir}")
    find_file(FMOD_STUDIO_DLL        NAMES fmodstudio.dll   HINTS ${_fmod_hints} PATH_SUFFIXES "api/studio/${_fmod_libdir}")
    find_file(FMOD_STUDIO_DLL_DEBUG  NAMES fmodstudioL.dll  HINTS ${_fmod_hints} PATH_SUFFIXES "api/studio/${_fmod_libdir}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FMOD
        REQUIRED_VARS
        FMOD_CORE_LIBRARY FMOD_CORE_INCLUDE_DIR
        FMOD_STUDIO_LIBRARY FMOD_STUDIO_INCLUDE_DIR)

mark_as_advanced(
        FMOD_CORE_INCLUDE_DIR FMOD_CORE_LIBRARY FMOD_CORE_LIBRARY_DEBUG FMOD_CORE_DLL FMOD_CORE_DLL_DEBUG
        FMOD_STUDIO_INCLUDE_DIR FMOD_STUDIO_LIBRARY FMOD_STUDIO_LIBRARY_DEBUG FMOD_STUDIO_DLL FMOD_STUDIO_DLL_DEBUG)

# 共有ライブラリのインポートターゲットを 1 つ作る。
#   TARGET  作るターゲット名
#   INC     インクルードディレクトリ
#   LIB     リリース用のライブラリ（Windows ではインポートライブラリ）
#   LIBD    Debug 用。無ければ LIB を使う
#   DLL     Windows のみ。実体の DLL
#   DLLD    Windows のみ。Debug 用の DLL
function(_fmod_add_imported TARGET INC LIB LIBD DLL DLLD)
    if(TARGET ${TARGET})
        return()
    endif()

    if(NOT LIBD)
        set(LIBD "${LIB}")
    endif()

    if(NOT DLLD)
        set(DLLD "${DLL}")
    endif()

    add_library(${TARGET} SHARED IMPORTED)
    set_target_properties(${TARGET} PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${INC}"
            IMPORTED_CONFIGURATIONS "RELEASE;DEBUG")

    if(WIN32)
        set_target_properties(${TARGET} PROPERTIES
                IMPORTED_IMPLIB "${LIB}"
                IMPORTED_IMPLIB_RELEASE "${LIB}"
                IMPORTED_IMPLIB_DEBUG "${LIBD}"
                IMPORTED_LOCATION "${DLL}"
                IMPORTED_LOCATION_RELEASE "${DLL}"
                IMPORTED_LOCATION_DEBUG "${DLLD}")
    else()
        set_target_properties(${TARGET} PROPERTIES
                IMPORTED_LOCATION "${LIB}"
                IMPORTED_LOCATION_RELEASE "${LIB}"
                IMPORTED_LOCATION_DEBUG "${LIBD}")
    endif()

    # RelWithDebInfo / MinSizeRel は FMOD 側に対応する構成が無いのでリリース扱いにする。
    set_target_properties(${TARGET} PROPERTIES
            MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
            MAP_IMPORTED_CONFIG_MINSIZEREL Release)
endfunction()

if(FMOD_FOUND)
    _fmod_add_imported(FMOD::Core "${FMOD_CORE_INCLUDE_DIR}"
            "${FMOD_CORE_LIBRARY}" "${FMOD_CORE_LIBRARY_DEBUG}"
            "${FMOD_CORE_DLL}" "${FMOD_CORE_DLL_DEBUG}")

    _fmod_add_imported(FMOD::Studio "${FMOD_STUDIO_INCLUDE_DIR}"
            "${FMOD_STUDIO_LIBRARY}" "${FMOD_STUDIO_LIBRARY_DEBUG}"
            "${FMOD_STUDIO_DLL}" "${FMOD_STUDIO_DLL_DEBUG}")

    # Studio API は Core の上に乗っているので、Studio をリンクすれば Core も付いてくる。
    set_property(TARGET FMOD::Studio APPEND PROPERTY INTERFACE_LINK_LIBRARIES FMOD::Core)
endif()

unset(_fmod_hints)
unset(_fmod_libdir)
