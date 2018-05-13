QR
====

## About
盤面の情報をQRコードから読み取ります。

## 使用ライブラリ
* OpenCV 3.4.1 Win pack
* ...

## インストール
### OpenCV

以下のサイトを参考にインストールします。
[OpenCV 3.4.1をVisual Studio 2017から使用する時の手順 - Qiita]( https://qiita.com/h-adachi/items/aad3401b8900438b2acd "OpenCV 3.4.1をVisual Studio 2017から使用する時の手順 - Qiita")

#### 手順
おおまかな手順を紹介します。

0. [https://opencv.org/releases.html](https://opencv.org/releases.html)から、v3.4.1の「Win pack」をダウンロード
0. ダウンロードした実行ファイルを実行し、解凍先は「C:\」を選択（Cドライブに「opencv」というフォルダが生成されます）
0. Visual Studioでプロジェクトを作成する
0. 「ソリューション　エクスプローラー」でプロジェクト名を右クリック->「プロパティ」をクリック
0. サイトを参考に設定（値は ; で区切る）
0. 適当なサンプルコードを実行