## 別講義の課題(tail.c)
### 問題文
tailコマンドを実装せよ．ただしオプションは考えなくても良い．
### 仕様
- オプション実装  
※オプションと入力の間にスペース必須．  
※コマンド引数において，第1引数に読み込みファイル，第2引数以降にオプションを入力すること．
    - [-n N]  
    ・最後からN行だけ表示させる．  
    ・指定なし（デフォルト）で10行．
    
### 実行例
```
$ ./a.out tail_readme.txt
with multi-byte-characters.
そしてこれはマルチバイト
文字があっても使用可能です．
But we should keep a rule 
to add a break at the last line, to add options after the second argument(the first argument is the file), 
to add " "(space) between "-" and argument in option.
ただし，最後の行には改行を加えておく，
コマンド引数1つ目に読み込みファイル，2つ目以降にオプションとする，
オプションについては「-」と入力値の間にスペースを置く，
といったルールを守っておく必要があります．
$ ./a.out tail_readme.txt -n 3
コマンド引数1つ目に読み込みファイル，2つ目以降にオプションとする，
オプションについては「-」と入力値の間にスペースを置く，
といったルールを守っておく必要があります．
$
```
