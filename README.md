CNSGLib サンプル3 Tレックスランナー
=====

概要
-----
コマンドプロンプト内で動くTレックスランナーです。  
自作ライブラリCNSGLibによって動作しています。  
GameClangがClangコンパイラ、GameBcc32がBcc32(bcc55)によってコンパイルされたものです。  

起動方法
-----
GameClang（ショートカット）を実行してください。  
レガシーコンソールの方がうまく動くかもしれません。

操作
-----
![title](./resources/screen.png)

- `SPACE` : ジャンプ
- `R` : リスタート

コンパイル
-----
本プログラムはGNU Makeでコンパイルできます。
- `make` : Clangでコンパイルします。
- `make bcc32` : Bcc32でコンパイルします。
- `make clean` : オブジェクトをクリーンします。
