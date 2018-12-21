#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LEN(arr) sizeof(arr)/sizeof(*arr)

typedef struct{
	int rowNum; // 行数
	int maxCSize; // 行の最大長※改行含む
}FileInfo;
typedef struct{
	int setRow; // 取得・表示する行数
	// ほかに開始行とかでも．
}TailOptions;

int strlen2break(char*);
void preGetFileInfo(FILE*, FileInfo*);


/*
[参考]
●コマンドライン引数
http://www.wakhok.ac.jp/~kanayama/C/02/node186.html
●fseek()
http://www9.plala.or.jp/sgwr-t/lib/fseek.html
*/
int main(int cinLen, char *cinStr[]){
	/*
	ファイルの読み込み
	*/
	FILE *fp;
	if((fp= fopen(cinStr[1], "r"))==NULL){
		printf("Can't find any file!\n");
		return -1;
	}


	/*
	ファイル（構成）情報を取得する前処理
	*/
	FileInfo fi;
	preGetFileInfo(fp, &fi);


	/*
	初期値設定
	*/
	TailOptions to;
	to.setRow = 10; // 取得・表示する行数
	char buf[to.setRow][fi.maxCSize]; // to.setRow行分の文字列を格納する
	char line[fi.maxCSize]; // 1行分の文字列を格納するバッファ
	int len2break = 0; // 各行における，'\n'までの文字数
	int rowCnt = 0; // （最後尾からの）行カウント
	int cp = -1; // ファイルにおけるポインタ（最後尾からなので負）


	/*
	オプションによる更新
	※「-」と入力値の間にスペース必要．
	※オプションはコマンド引数の2つ目以降に書くこと．
	*/
	if(cinLen>2){ // オプションがある場合はcinLenは2より大きいはず？
		int cinIndex = 2;
		while(cinIndex<cinLen){
			if(!strcmp(cinStr[cinIndex++], "-n")){
				to.setRow = atoi(cinStr[cinIndex++]);
			}
		}
	}


	/*
	基本処理
	*/
	fseek(fp, cp, SEEK_END); // 最後尾から1つ前の位置に移動．
	if(fgetc(fp)=='\n'){ // -1に改行ならば次にポインタを動かす．0はどのみち'EOF'になる．
		cp--;
	}

	while(rowCnt<to.setRow){
		if(fseek(fp, --cp, SEEK_END)!=0){ // 先頭に到達したらその行を出力して終了
			fseek(fp, cp+1, SEEK_END);
			fgets(line, fi.maxCSize, fp);
			len2break = strlen2break(line);
			line[len2break-1] = '\0';
			strcpy(buf[rowCnt++], line);
			break;
		}

		if(fgetc(fp)=='\n'){
			fseek(fp, cp+1, SEEK_END); // '\n'の直後の文字にポインタを移す
			fgets(line, fi.maxCSize, fp);
			len2break = strlen2break(line);
			line[len2break-1] = '\0';
			strcpy(buf[rowCnt++], line);
		}
	}


	/*
	ファイル操作の変数を閉じる
	*/
	fclose(fp);


	/*
	出力
	*/
	int i;
	for(i=rowCnt-1; 0<=i; i--){
		printf("%s\n", buf[i]);
	}

	return 0;
}


/*
'\n'までの文字数をカウントする．
※'\n'もカウントに含む．
*/
int strlen2break(char *line){
	int cnt = 0;
	while(1){
		if(*(line++)=='\n') return cnt+1;
		cnt++;
	}
}


/*
ファイル（構成）情報を取得する
*/
void preGetFileInfo(FILE *fp, FileInfo *fi){
	int rowNum = 0; // 行数
	int cnum = 0; // 1行の文字数　※改行含む
	int maxSize = 0;
	char fc; // ファイルから読み込む1文字

	while((fc=fgetc(fp)) != EOF){ // 1文字ずつ調べる
		if(fc=='\n'){
			// 行の加算
			rowNum++;

			// 最大長の更新
			if(cnum > maxSize){ // 改行の分をここで加算して行の最大長を更新
				maxSize = cnum;
			}
			// 文字数の初期化
			cnum = 0;
		}else{
			cnum++; // ※マルチバイト文字は3byte/文字でカウントされる模様．どのみち余分に確保されるのでこのままで．
		}
	}

	// ファイル（構成）情報の構造体に登録
	fi->rowNum = rowNum;
	fi->maxCSize = maxSize;
}