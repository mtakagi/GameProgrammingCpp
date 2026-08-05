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
| Chapter04 | A\* 経路探索、状態機械による AI、グリッドとタイル | タワーディフェンス | 実装中 |

## 操作

| 章 | キー / マウス |
|---|---|
| Chapter01 | `W` / `S` でパドル移動 |
| Chapter02 | `W` `A` `S` `D` で移動 |
| Chapter03 | `W` / `S` で前後、`A` / `D` で旋回、`Space` でレーザー |
| Chapter04 | 左クリックでタイル選択、`B` で選択タイルに塔を建設 |

いずれも `Esc` で終了します。

## 必要なもの

- CMake 3.26 以上
- C++23 対応のコンパイラ（MSVC 19.4x / GCC 13 / Clang 16 以降で確認）
- git（依存ライブラリの取得に使用）

SDL2 / SDL2_image は CMake の FetchContent がソースから取得してビルドするため、
個別にインストールする必要はありません。

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
```

Chapter01 はテクスチャを使わないためアセット不要です。

## 動作環境

Windows 11 + CLion（Ninja + MSVC）で開発し、GitHub Actions で
Windows / Ubuntu / macOS の 3 環境のビルドを確認しています。

## ライセンス

書籍のコードを写経した、学習目的のリポジトリです。元コードの権利は原著者に帰属します。
