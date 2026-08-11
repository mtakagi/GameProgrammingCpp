# GameProgrammingCpp

[![CI](https://github.com/mtakagi/GameProgrammingCpp/actions/workflows/ci.yml/badge.svg)](https://github.com/mtakagi/GameProgrammingCpp/actions/workflows/ci.yml)

Sanjay Madhav『ゲームプログラミング C++』を写経しながら進める学習用リポジトリです。
書籍のコードをそのまま追うのではなく、**CMake + C++23** に置き換えて実装しています。

章ごとに独立した実行ファイルを作り、章間でコードは共有していません
（本の進行どおり、前章からコピーして育てる方式）。

## 書籍

- 日本語版: [『ゲームプログラミング C++』](https://www.shoeisha.co.jp/book/detail/9784798157610)
  Sanjay Madhav 著 / 吉川 邦夫 訳 / 今給黎 隆 監修、翔泳社、2018 年 12 月、ISBN 978-4-7981-5761-0
- 原書: *Game Programming in C++: Creating 3D Games*（Addison-Wesley, 2018）
- 書籍のサンプルコード: [gameprogcpp/code](https://github.com/gameprogcpp/code)

## 進捗

| 章 | 内容 | 作るもの | 状態 |
|---|---|---|---|
| Chapter01 | ゲームループ、SDL の初期化 | Pong | 完了 |
| Chapter02 | ゲームオブジェクトとコンポーネント、スプライト描画・アニメーション・スクロール背景 | 横スクロール | 完了 |
| Chapter03 | ベクトル演算、`MoveComponent` / `InputComponent`、円判定による衝突 | Asteroids | 完了 |
| Chapter04 | A\* 経路探索、状態機械による AI、グリッドとタイル | タワーディフェンス | 完了 |
| Chapter05 | OpenGL への移行、頂点／インデックスバッファ、シェーダー、行列変換 | Asteroids（OpenGL 版） | 完了 |
| Chapter06 | 3D 描画、ビュー・射影行列、クォータニオン回転、`.gpmesh` 読み込み、Phong シェーディング | 3D シーン | 完了 |
| Chapter07 | FMOD Studio によるオーディオ、3D 音源、バス・スナップショット | 3D シーン + サウンド | 実装中 |

## 操作

| 章 | キー / マウス |
|---|---|
| Chapter01 | `W` / `S` でパドル移動 |
| Chapter02 | `W` `A` `S` `D` で移動 |
| Chapter03 | `W` / `S` で前後、`A` / `D` で旋回、`Space` でレーザー |
| Chapter04 | 左クリックでタイル選択、`B` で選択タイルに塔を建設 |
| Chapter05 | `W` / `S` で前後、`A` / `D` で旋回、`Space` でレーザー |
| Chapter06 | `W` / `S` でカメラ前後、`A` / `D` でカメラ旋回 |
| Chapter07 | Chapter06 の操作に加えて下記 |

Chapter07 のオーディオ操作:

| キー | 動作 |
|---|---|
| `-` / `=` | マスターバスの音量を 0.1 ずつ下げる / 上げる |
| `E` | 効果音 `event:/Explosion2D` を再生 |
| `M` | BGM の一時停止をトグル |
| `R` | リバーブのスナップショットをトグル |
| `1` / `2` | 足音のサーフェス種別を切り替え |

いずれも `Esc` で終了します。

## 必要なもの

- CMake 3.26 以上
- C++23 対応のコンパイラ（MSVC 19.4x / GCC 13 / Clang 16 以降で確認）
- git（依存ライブラリの取得に使用）

次の依存ライブラリは CMake の FetchContent がソース（または配布アーカイブ）から
取得してビルドするため、個別にインストールする必要はありません。

| ライブラリ | 用途 | 取得方法 |
|---|---|---|
| SDL2 2.32.10 | ウィンドウ・入力・タイマー | git |
| SDL2_image 2.8.12 | 画像読み込み（Chapter02〜04） | git |
| GLEW 2.3.1 | OpenGL の拡張関数取得（Chapter05〜） | リリース版 zip |
| SOIL | OpenGL 用の画像読み込み（Chapter05〜） | git |
| RapidJSON | `.gpmesh` の解析（Chapter06〜） | git |

### FMOD（Chapter07 以降のみ）

Chapter07 のオーディオには [FMOD Studio API](https://www.fmod.com/download) が必要です。
**再配布できないライブラリのため FetchContent では取得できません。** 各自でインストールしてください
（個人・非商用は無償ライセンスの対象です）。

`cmake/FindFMOD.cmake` が既定のインストール先を探します。別の場所に入れた場合は
`FMOD_ROOT` で指定してください。

```sh
cmake -B build -DFMOD_ROOT="D:/SDK/FMOD Studio API Windows"
```

**FMOD が見つからない場合、Chapter07 はビルド対象から自動的に外れます。**
他の章の確認は妨げません（CI もこの動作に依存しています）。

なお書籍は FMOD 1.10 を対象にしているため、2.x では API 名が変わっています
（`getLowLevelSystem` → `getCoreSystem`、`setParameterValue` → `setParameterByName` など）。

Linux では SDL のビルドに開発パッケージが必要です:

```sh
sudo apt-get install -y \
  libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev \
  libxinerama-dev libxss-dev libxkbcommon-dev \
  libwayland-dev wayland-protocols libegl1-mesa-dev libgl1-mesa-dev \
  libasound2-dev libpulse-dev libudev-dev
```

## ビルド

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug --parallel
```

特定の章だけをビルドする場合は `--target` を指定します。

```sh
cmake --build build --config Debug --target Chapter04 --parallel
```

実行ファイルは `build/ChapterNN/` に出力されます。Windows では依存 DLL が、
アセットがあればアセットも同じ場所へ自動でコピーされます。

## アセットについて

**書籍付属の画像アセットはこのリポジトリに含まれていません**（`.gitignore` で除外しています）。
無い状態でもビルドは通りますが、テクスチャを読み込む章は実行時に画像を開けません。

書籍のサンプルコード [gameprogcpp/code](https://github.com/gameprogcpp/code) から
該当章の `Assets` を取得し、次の場所に配置してください。

```
Chapter02/Assets/
Chapter03/Assets/
Chapter04/Assets/
Chapter05/Assets/  Chapter05/Shaders/
Chapter06/Assets/  Chapter06/Shaders/
Chapter07/Assets/  Chapter07/Shaders/
```

Chapter01 はテクスチャを使わないためアセット不要です。
Chapter07 の `Assets/` には FMOD のバンクファイル（`Master Bank.bank`、
`Master Bank.strings.bank`）も必要です。

## 動作環境

Windows 11 + CLion（Ninja + MSVC）で開発し、GitHub Actions で
Windows / Ubuntu / macOS の 3 環境のビルドを確認しています。

CI のランナーには FMOD が無いため、**Chapter07 以降は CI でビルドされません**。
Chapter06 までは Ubuntu ジョブが「ヘッダーが自分の使う型を自分で include しているか」を
検出する役割を果たしていましたが（`<cstdint>` や `<algorithm>` の漏れを何度か拾っています）、
第 7 章からはその検査が効かない点に注意してください。

## ライセンス

書籍のコードを写経した、学習目的のリポジトリです。元コードの権利は原著者に帰属します。
