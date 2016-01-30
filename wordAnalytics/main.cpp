#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <assert.h>
#include <stdlib.h>

#include "main.h"

//プロトタイプ宣言
int createrandomarray(int outputnumberarray[], int n);
std::string replace_text(std::string target, std::string find, std::string replace);

int main(void) {

	/////////////////////////////////////////////////////////////
	//			単語入力
	//			最大１０個
	//			"EXIT"入力で入力終了
	/////////////////////////////////////////////////////////////
	int counter1 = 0;
	std::string tempstring = "";	//単語入力用一時変数
	std::string word_list[9];		//入力された単語の配列

	//メッセージ出力
	std::cout << "言葉を入力してください（最大１０個）\n終了する場合は\"EXIT\"を入力\n※単語が重複しないようご注意ください\n\n";

	//単語入力処理
	while (counter1 <= 10 && tempstring != "EXIT" && tempstring != "exit") {

		counter1++;	//入力された個数

		std::cout << counter1 << ':';
		std::getline(std::cin, tempstring);

		if (tempstring != "EXIT" && tempstring != "exit") {
			word_list[counter1 - 1] = tempstring;
		}
		else {
			counter1--;
		}
	}

	//入力された単語を画面に出力
	std::cout << "入力された言葉\n";

	for (int i = 1; i <= counter1; i++) {
		std::cout << word_list[i - 1] << " ";
	}

	std::cout << "\n";

	////////////////////////////////////////////////////////////
	//		質問準備
	////////////////////////////////////////////////////////////
	int *numberarray1 = new int[counter1 * counter1 - 1];	//組み合わせ番号の１つ目
	int *numberarray2 = new int[counter1 * counter1 - 1];	//組み合わせ番号の２つ目

	 //動的な２次元配列を作成
	 //スコア記録用２次元配列
	int **score_array = new int*[counter1 - 1];
	for (int i = 0; i < counter1; i++) {
		score_array[i] = new int[counter1 - 1];
	}

	int counter2 = 0;
	for (int i = 0; i < counter1; i++) {
		for (int j = 0; j < counter1; j++) {
			//組み合わせ番号の配列
			numberarray1[counter2] = i;
			numberarray2[counter2] = j;
			//スコアを初期化
			score_array[i][j] = -1;

			counter2++;
		}
	}

	//乱数配列作成
	int *randomnumberarray = new int[counter1*counter1 - 1];
	createrandomarray(randomnumberarray, counter1*counter1 - 1);

	//比較に使用する単語の番号
	int wordnumber1;
	int wordnumber2;

	////////////////////////////////////////////////////////////////////////
	//ランダムに組み合わせを表示して関係性を質問する
	////////////////////////////////////////////////////////////////////////

	//スコア
	int score1;

	//総当たりで２つづず単語を表示し、関係性を入力してもらう
	for (int i = 0; i < counter1 * counter1; i++) {

		int tmpnumber1;//乱数配列から取得した乱数
		tmpnumber1 = randomnumberarray[i];

		score1 = 0;
		wordnumber1 = numberarray1[tmpnumber1];
		wordnumber2 = numberarray2[tmpnumber1];

		if (wordnumber1 != wordnumber2) {
			if (-1 != score_array[wordnumber1][wordnumber2]) {
				
				refresh_console();	//画面をクリアする

				//２つの単語を表示する
				std::cout << "\n     " << word_list[wordnumber1] << "     " << word_list[wordnumber2] << "\n\n";
				std::cout << "  0 : 関係ない\n  1 : 少し関係ある\n  2 : とても関係がある\n\n番号を入力:";

				//ユーザーに数値を入力してもらう
				std::cin >> score1;

				//入力チェック
				while (2 < score1 || 0 > score1) {
					std::cout << "0,1,2のどれかを入力してください\n番号を入力:";
					std::cin >> score1;
				}
			}
		}
		else {
			//同じ単語の場合は交差部のスコアを入れる
			score1 = intersectpoint1;
		}
		//スコアをスコア配列に入れる
		score_array[wordnumber1][wordnumber2] = score1;
		score_array[wordnumber2][wordnumber1] = score1;
	}

	refresh_console();	//画面をクリアする

	///////////////////////////////////////////////////////////////////////
	//	スコア合計が閾値以下の単語を削除する
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	//		ディレクトリ準備
	///////////////////////////////////////////////////////////////////////
	//一時フォルダ作成
	create_directory(tmpdir1);
	//結果フォルダ作成
	create_directory(resultdir1);

	///////////////////////////////////////////////////////////////////////
	//	スコア表をcsvで出力
	///////////////////////////////////////////////////////////////////////
	
	std::ofstream out_file(tmpdir1 + sep + csvfilename1);

	for (int i = 0; i < counter1; i++) {
		out_file << "," << word_list[i];
	}
	out_file << "\n";
	for (int i = 0; i < counter1; i++) {
		out_file << word_list[i];
		for (int j = 0; j < counter1; j++) {
			out_file << "," << score_array[i][j];
		}
		out_file << "\n";
	}
	out_file.close();

	///////////////////////////////////////////////////////////////////////
	//	Rのスクリプトファイルを作成
	///////////////////////////////////////////////////////////////////////
	std::string r_script_fullpath = create_r_script();

	///////////////////////////////////////////////////////////////////////
	//	rscript.exeの場所を選択
	//	!!!!!!!! 定数で対応 !!!!!!!!!
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	//	R実行用のバッチファイル作成
	///////////////////////////////////////////////////////////////////////
	std::string batch_path = create_r_batch(r_script_fullpath);

	///////////////////////////////////////////////////////////////////////
	//	バッチファイル実行
	///////////////////////////////////////////////////////////////////////
	std::cout << "対応分析開始";
	command_execute(batch_path);
	std::cout << "対応分析終了\n";

	///////////////////////////////////////////////////////////////////////
	//	終了用メッセージを出力
	///////////////////////////////////////////////////////////////////////
	std::cout << "-------------------------------------------------------------------------------------\n";
	std::cout << "入力された言葉の座標値を\"" << tmpdir1 << sep << correspondence_result_filename << "\"に出力しました\n";
	std::cout << "Excel等で散布図を作成してください\n";

	///////////////////////////////////////////////////////////////////////
	//	後処理
	///////////////////////////////////////////////////////////////////////
	/*
	delete[] randomnumberarray;
	for (int i = 0; i < counter1; i++) {
	delete[] score_array[i];
	}
	delete[] score_array;
	delete[] numberarray2;
	delete[] numberarray1;
	*/
	system("pause");
	return 0;
}

//////////////////////////////////////////////////////////////////
//			0～nのランダムな並びの連番配列を作成
//				ouputnumberarray = 出力用配列
//				n = 配列の長さ
//////////////////////////////////////////////////////////////////
int createrandomarray(int outputnumberarray[], int n) {

	float *randomarray = new float[n];//乱数を入れる配列

	std::random_device rnd;//ランダム作成用

	for (int i = 0; i <= n; i++) {
		randomarray[i] = rnd();
	}

	for (int i = 0; i <= n; i++) {

		float tmpnum = randomarray[i];
		int counter1 = 0;//tmpnumより小さい数字の数

						 //ランダム配列の中の自分以下の数字の個数を順位とする
		for (int j = 0; j <= n; j++) {
			if (randomarray[j] < tmpnum) {
				counter1++;
			}
		}

		//順位を出力配列に入れる
		outputnumberarray[i] = counter1;

	}

	//delete[] randomarray;

	return 0;

}

//////////////////////////////////////////////////////////////////
//			rのスクリプトファイルを作成
//				戻り値：スクリプトファイルのフルパス
//
//				スクリプト内容：Rで対応分析を行って結果を出力する
//////////////////////////////////////////////////////////////////
std::string create_r_script(void) {

	//カレントフォルダ取得
	std::string current_directory = get_current_directory();

	std::string script_directory_path_slash = replace_text(current_directory, sep, "/") + "/" + tmpdir1;

	std::ofstream out_file(tmpdir1 + sep + scriptname1);

	out_file << "setwd (\"" << script_directory_path_slash << "\")" << "\n";
	out_file << "table.N <- as.matrix(read.table(\"" << csvfilename1 << "\", sep=\",\", header=TRUE, row.names = 1))" << "\n";
	out_file << "table.P <- table.N / sum(table.N)" << "\n";
	out_file << "r <- apply(table.P, 1, sum)" << "\n";
	out_file << "c <- apply(table.P, 2, sum)" << "\n";
	out_file << "Drmh <- diag(1/sqrt(r))" << "\n";
	out_file << "Dcmh <- diag(1/sqrt(c))" << "\n";
	out_file << "S <- Drmh %*% (table.P -r %o% c) %*% Dcmh" << "\n";
	out_file << "S.svd <- svd(S)" << "\n";
	out_file << "F <- Drmh %*% S.svd$u %*% diag(S.svd$d)" << "\n";
	out_file << "G <- Dcmh %*% S.svd$v %*% diag(S.svd$d)" << "\n";
	out_file << "columns1 <- 1:ncol(F)" << "\n";
	out_file << "columns1[1] <- \"X\"" << "\n";
	out_file << "columns1[2] <- \"Y\"" << "\n";
	out_file << "dimnames(F) <- list(colnames(table.N), columns1)" << "\n";
	out_file << "dimnames(G) <- list(colnames(table.N), columns1)" << "\n";
	out_file << "write.table(F[ , c(1, 2)], \"../" << resultdir1 << "/" << correspondence_result_filename << "\", sep=\",\",col.names=NA)" << "\n";
	//out_file << "write.table(F[ , c(1, 2)], \"../" << resultdir1 << "/vertical.csv\", sep=\",\",col.names=NA)" << "\n";
	//out_file << "write.table(G[ , c(1, 2)], \"../" << resultdir1 << "/holizontal.csv\", sep=\",\",col.names=NA)" << "\n";
	out_file << "write.table(S.svd$d, \"singular.csv\", sep=\",\")" << "\n";

	out_file.close();

	return current_directory + sep + tmpdir1 + sep + scriptname1;
}

//////////////////////////////////////////////////////////////////
//			文字列内の文字の全置換
//				target=置換対象文字列
//				find=置換する
//				replace=置換後の文字列
//////////////////////////////////////////////////////////////////
std::string replace_text(std::string target, std::string find, std::string replace) {
	std::string::size_type position = target.find(find);
	while (position != std::string::npos) {
		target.replace(position, find.length(), replace);
		position = target.find(find);
	}
	return target;
}
