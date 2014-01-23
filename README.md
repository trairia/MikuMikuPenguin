**WARNING**
THIS FILE IS ENCODED IN UTF-8, NOT SJIS!!!

**注意**
このファイルはSJISではなくUTF-8でエンコードされています！！！

# LibMikuMikuPenguin(libmmp) v0.1 リードミ（日本語）

English translation is [available](http://github.com/sn0w75/mikumikupenguin/blob/master/README.en.md).

どうも初めまして。sn0w75です。

自分の好きなOSでミクミクしたくて、それを出来る為のライブラリやプログラムを作ってみました。
まだバグだらけですし、APIが確定しているわけでもないのでMMPをライブラリとして使うのはまだまだ早いかと思います。
ですが、これを起点として、Windows以外のOSでのMMDの開発が段々進めていったら～と思ってます。

最新のコードは以下のリンクで手に入れます：
https://github.com/sn0w75/MikuMikuPenguin/

## ディレクトリ構成

中身はこんな感じです：

* COPYING: このコードのライセンス
* doc: ドキュメンテーションを入れる為のフォルダですが、現時点では得に何も入ってません。
* include: LibMikuMikuPenguinのヘッダ（.h）ファイルがここに入ってます。
* INSTALL: このコードのインストール仕方（英語のみ）
* libmmp: LibMikuMikuPenguinのソース（.cpp）ファイルがここに入ってます。
* shaders: PMXViewerのシェーダーがここに入ってます。
* src: PMXViewerのソースコード（.cpp、.h）がここに入ってます。
* data: 必要なテクスチャ等が入ってます。

## 依存ライブラリ

LibMikuMikuPenguinにはlibglfw, libglew,libglm,libBulletが必要です。

PMXViewerにはlibmmp、libSOILが必要です。VMDと一緒に音楽を再生したい場合は、SDL2、SDL2_mixerも必要です。（なくてもコンパイル出来ます）

### ライブラリの詳細（2014年1月20日）：

* libglfw: 現在では2.7が必要です。3.0+は2.xと非対応です。
* libglm: 現在では0.9.4(0.9.2、 0.9.3でも大丈夫かもしれません）が必要です。0.9.5+は0.9.4と非対応です。

**注意！！！！！**

GLM 0.9.3（<=0.9.2もそうかもしれない）ではバグがあります： 
https://github.com/g-truc/glm/issues/15

このバグを回避する為には、-fpermissiveを使う必要があります。例えば：

    $ export CXXFLAGS="-fpermissive"
    $ autoreconf -i
    $ ./configure
    # make install

みたいな感じです。

## インストール
このソースコードはautotoolsを利用していますので、コンパイルやインストールは簡単です：

コンパイルするにはターミナルで：

    $ autoreconf -i
    $ ./configure
    $ make

をすれば大丈夫です。
またシステムにインストールしたい場合は：

    $ sudo make install

みたいな感じで大丈夫です。

他に質問がありました、気軽にメッセージ飛ばしてください。喜んで手伝います：
* Twitter [@sn0w75](http://twitter.com/sn0w75)
* E-mail <ibenrunnin@gmail.com>

