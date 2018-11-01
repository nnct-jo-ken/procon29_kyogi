第29回高専プロコン　競技部門
====

本リポジトリは、本番で使用したプログラムとはほとんど関係ありません。  
以下のリポジトリが実際に使用したプログラムです。

* [GUI](https://github.com/nnct-jo-ken/procon2018_kyogi_GUI)
* [ソルバー](https://github.com/nnct-jo-ken/procon2018_kyogi_solver)

## Description
[募集要項](http://www.procon.gr.jp/wp-content/uploads//2016/12/266299c2bb26295c0ada037e8f235cb4.pdf "第29回高専プロコン　募集要項")

## Investment policy

### ブランチの役割
「Git flow」のようなブランチの管理をします。

* master
	- メインブランチ
	- 安定版のみ
* develop
	- 開発ブランチ
* feature/{機能名}
	- 機能の追加
	- developから分岐し、developにマージ
* hotfix
	- masterから分岐し、masterとdevelopにマージ

### コミット
#### コミットメッセージ
日本語でも可です。わかりやすいメッセージをつけてください。

#### タイミング
機能の追加や修正、削除など、細かい単位でコミットしてください。

### コードを変更する場合
他人のコードと競合が起きた場合は、その人と話し合います。
全体に影響することであれば、全体に連絡します。

### 新しい機能を開発し始めるとき
```
git pull origin 
git checkout develop
git checkout -b feature/{機能名}
```

### 機能開発を終えるとき
'feature/{機能名}'ブランチにて開発を終了する時は、以下のようにしてください。

```
git commit -m 'commit message'
git push origin
```

push後に、Pull Requestを発行してマージの申請をしてください。
問題がなければ、マージされます。

### issueの使い方
実装する機能、バグなどについて、具体的なモノをissueとして作成してください。
