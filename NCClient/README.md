NCClient
====

### 概要
UDP通信を用いてカメラ・音声データの中継を行うクライアント機能

#### 説明
NCServer に接続した最大9名のカメラ画像と音声データの送受信を行います

#### 依存関係
OpenCV を使用しているため
cxcore110.dll、highgui110.dll、OpenCV.dll  
の3つ必要となります

#### 使い方
NCClient.cpp の36行目にあるブロードキャスト用のIPアドレスをNCServer起動時に登録するブロードキャスト用のIPアドレスに指定します  
EXEファイル実行後、ダイアログが表示されるのでユニークの名前を登録し開始してください

#### インストール

#### ライセンス

#### 著者

[kaztoyoshi](https://github.com/kaztoyoshi)
