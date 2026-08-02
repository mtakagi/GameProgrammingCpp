# CLAUDE.md

## このリポジトリについて

Sanjay Madhav『Game Programming in C++（ゲームプログラミング C++）』を写経しながら進める学習用リポジトリ。
章ごとに独立した実行ファイルを作る（`Chapter01/`, `Chapter02/`, ...）。章間でコードは共有せず、
本の進行どおり前章からコピーして育てる。

- 言語: C++23 / 回答は日本語
- 依存: SDL2 2.32.10, SDL2_image 2.8.12（どちらも FetchContent でソース取得）
- 環境: Windows 11 + CLion 2026.1（Ninja + MSVC 14.50）

## 学習方針（重要）

- **章のソースコード（`ChapterNN/*.cpp` / `*.h`）は本人が書く。** Claude から先回りして
  実装を書き込まない。エラーの原因、言語仕様、設計意図の説明を優先する。
  実装例を示す場合は、ファイル編集ではなく回答内のコードブロックで提示する。
- **ビルド周り（`CMakeLists.txt`、ツールチェーン、環境問題）は Claude が直してよい。**
  ここは学習対象ではないため。
- エラーメッセージを貼られた場合は、まず**根本原因の特定**を返す。表面のエラー行と
  原因箇所がずれていることが多い（例: 不完全型が原因で、無関係な基底クラス変換エラーが出る）。

## ビルド

CLion からビルド・実行するのが通常。コマンドラインから叩く場合は
**MSVC の開発者環境を読み込まないと `cl.exe` が `stddef.h` を見つけられない**ので注意:

```
cmd /c '"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul && cmake --build cmake-build-debug --target Chapter02 -j 14'
```

構成（configure）は環境変数なしでも通る。

## CMake の構成

- ルート `CMakeLists.txt` に `add_chapter()` 関数を定義し、章側はその呼び出し 1 つだけにする。

  ```cmake
  add_chapter(Chapter02
          SOURCES Game.cpp Actor.cpp ...
          LIBS SDL2_image::SDL2_image
          ASSET_DIRS Assets)
  ```

  `add_chapter` が実行ファイル作成・SDL2 リンク・DLL コピー・Assets コピーをまとめて行う。
- 自分のターゲット共通のコンパイル設定は INTERFACE ライブラリ `project_options` に集約する。
  `add_compile_options` / `CMAKE_CXX_STANDARD` のようなディレクトリスコープの設定は使わない
  （FetchContent で追加される SDL 側のビルドまで巻き込むため）。
- `cmake_minimum_required` は使っている機能に合わせる。現在 3.26
  （`$<TARGET_RUNTIME_DLLS>` = 3.21、`copy_directory_if_different` = 3.26）。

### 新しい章を追加する手順

1. `ChapterNN/` を作り、前章からソースをコピーする
2. `ChapterNN/CMakeLists.txt` に `add_chapter(ChapterNN SOURCES ...)` を書く
3. ルート `CMakeLists.txt` に `add_subdirectory(ChapterNN)` を追加する

## この環境で踏んだ問題（再発時の参照用）

- **Smart App Control がローカルビルドの exe をブロックする**
  `CreateProcess error=4551` / CodeIntegrity ログ ID 3077。SAC は署名とクラウド評価で
  ファイル単位に判定するため、同じ設定でも exe ごとに結果が変わる。再リンクすると通ることが多い。
  恒久対策は SAC をオフにすることだが**一度オフにすると再有効化には Windows の再インストールが必要**。
  除外リストは存在しない。設定変更は本人が判断する（Claude は行わない）。
- **SDL2_image の AVIF が NASM を要求して configure が失敗する**
  vendored dav1d のビルドに NASM が必要。本書では PNG しか使わないので
  `SDL2IMAGE_AVIF/JXL/TIF/WEBP` を OFF にしてある。PNG / JPEG は stb_image バックエンドで読める。
- **`0xC0000135`（DLL not found）で exe が即終了する**
  Windows は RPATH を持たないため、依存 DLL を exe の隣に置く必要がある。
  `add_chapter` の `$<TARGET_RUNTIME_DLLS>` コピーで対応済み。
- **`Vector2` などを前方宣言だけで値メンバーにすると、無関係な場所にエラーが出る**
  クラス定義が不正になり、派生クラスから基底への変換エラーとして表示される。
  共通の型は `Math.h` に置いてインクルードする。

## macOS / Linux

`if(MSVC)` と `if(WIN32)` でガード済みのため、そのまま構成できる想定。
Linux では SDL のビルドに X11 / Wayland の開発パッケージが必要
（無いと configure は通るが実行時に `SDL_Init(SDL_INIT_VIDEO)` が失敗する）。
