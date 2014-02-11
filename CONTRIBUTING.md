# How to contribute

第三者による寄稿はこれからのMikuMikuPenguinの開発には必須なものです。
自分の環境でMMPを動かせる為の変更を出来るだけ易しく寄稿出来るようにしたい。
その為には、寄稿者に指導方針に従うようお願い致します。

## Getting Started

* 寄稿する為には[GitHubアカウント](https://github.com/signup/free)が必要です。
* 問題にはIssueを作ってください（もうIssueが作られていない場合）。
  * バグをissueで報告する場合、バグを再現する為の説明を書いてください。
  * Issueのタイトルは英語が推奨ですが日本語でも構いません。
  * 本文は自国語でお願いします。つまり日本の方なら日本語でお願いします。
* GitHubでレポジトリのforkを作ってください。

## Making Changes

* 寄稿のベースとなるブランチから話題ブランチを作ってください。
  * 普段ならこれはmasterブランチです。
  * masterブランチから話題ブランチを作るには： `git branch
    my_contribution master` してから `git
    checkout my_contribution`で新しいブランチに切り替えます。直接masterブランチで作業するのを避けてください。
* commitは合理的に分けてください。
* commitする前に、不要な空白は `git diff --check` で確認してください。
* commitメッセージが正しいフォーマットにあることを確認してください。commitメッセージは出来るだけ英語でお願いします。

````
必須なコミット情報はここに（出来れば英語で）。

上にある最初の列はパッチ/コミットの概要を説明します。
この本文はパッチが無い状態のプログラムの行動、なんでこの行動は問題なのか、どうやってパッチが問題を解決するのかを説明します。
````

* 変更の為にテストが必要ならそのテストが追加されているよう確認してください。
* 「make check」で何かが無意に壊れなかったか確認してください。他にテストがあればそれも実行してみてください。

## Coding Style
基本的に以下のようなスタイルでコードを書いてくださるようお願いします。
このスタイルに合わせなくても結構ですが推奨です。
```cpp
int variableName;

class ClassName
{
	public:
	ClassName();
	~ClassName();

	void classFunctionName();

	private:
	int classVariableName;
};

void functionName()
{
	//function contents...
}
```

概ね、「variable_name」より「variableName」を使います。
クラス名の最初の字（ClassNameの「C」）は大文字です。
ブラケットは普段別々の列で開かれ閉ざせます（以下の例と違って：）
```cpp
void functionName() { //Bracket is opened on the same line as the class/function name. Typically unused style in MMP code
	//Function contents here...
};
```

コメントはcommitメッセージと違って、英語でも日本語でも構いません。英語に自信ない方は日本語を使ってください。
理由としては、コメントで間違った英語を使うと英語圏の人でも日本語圏の人でも、どっちでも理解出来ないコメントになってしまう恐れがあるからです。
そういう場合は日本語圏の人にコードを理解して貰うだけでもまだマシだと思いますので、自信がある言語でコメントを書いてくださるようお願いします。

## Making Trivial Changes

### Documentation

コメントやドキュメンテーションのマイナーな変更には、commitメッセージを
「(doc)」で始めることをお勧めします。

````
(doc) Add documentation commit example to CONTRIBUTING

There is no example for contributing a documentation commit
to the MMP repository. This is a problem because the contributor
is left to assume how a commit of this nature may appear.

The first line summarizes the patch content, starting with '(doc)'
at the beginning of the commit. The body describes the nature of
the new documentation or comments added.
````

## Submitting Changes

* 自分のforkで、変更を話題ブランチにpushしてください。
* MikuMikuPenguinのレポジトリにpull requestを投稿してください。

# Additional Resources

* [General GitHub documentation](http://help.github.com/)
* [GitHub pull request documentation](http://help.github.com/send-pull-requests/)
* sn0w75を連絡： (http://twitter.com/sn0w75), メール (ibenrunnin@gmail.com)

