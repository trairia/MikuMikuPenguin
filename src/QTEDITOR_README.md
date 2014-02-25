

Qtエディタをレポジトリで実装しました。
Autotoolsからcmakeに移ることになります。
ビルドするには、ルートディレクトリで：
     cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/cmake/files
     make
をすればビルド出来るはずです。

必要なライブラリはPMXViewerとほぼ同じですが、Qt 5.2が必要になります。
Qt 5.2はここからダウンロード出来ます：
http://qt-project.org/wiki/Qt-5.2-release

ダウンロードした後：
cmake -DCMAKE_PREFIX_PATH=/home/sn0w75/Applications/Qt/5.2.1/gcc_64/lib/cmake
という風にcmakeを使えば、Qtのライブラリを見つけられるはずです。

cmakeのビルド環境はまだ初歩的なものです。よってPMXViewerとQtエディタが同時にコンパイルされます。

他に質問があればsn0w75氏に連絡：
http://twitter.com/sn0w75
