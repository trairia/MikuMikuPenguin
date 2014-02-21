

QTエディタを適当にレポジトリに入れました。
PMXViewerはとりあえず適当に「demos/PMXViewer」に投げました。
（PMXViewerやQTエディタに新しいレポジトリを作った方がいいかもしれません…最終的にどうするべきか迷う所です。）

まだAutotoolsと繋がってません。
（Autotoolsでmocを使うのは難しい為、cmakeに変える必要があるかもしれません。）

今の所、コンパイルするには、srcダイレクトリで：
     qmake
     make
     ./qt-editor

を実行すれば大丈夫です。
QT 5.1以上が必要です。

他に質問があればsn0w75氏に連絡：
http://twitter.com/sn0w75
