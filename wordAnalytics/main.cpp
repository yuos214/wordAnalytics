#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <assert.h>
#include <stdlib.h>

#include "main.h"

//プロトタイプ宣言
int createrandomarray(int outputnumberarray[], int n);

int main(void) {

	/////////////////////////////////////////////////////////////
	//			単語入力
	//			最大１０個
	//			"EXIT"入力で入力終了
	/////////////////////////////////////////////////////////////
	int counter1 = 0;
	std::string tempstring = "";	//単語入力用一時変数
	std::string word_list[9];	//入力された単語の配列

	std::cout << "文字列を入力してください（最大１０個）\n終了する場合は\"EXIT\"を入力\n";
	while (counter1 <= 10 && tempstring != "EXIT") {

		counter1++;	//入力された個数

		std::cout << counter1 << ':';
		std::getline(std::cin, tempstring);

		if (tempstring != "EXIT") {
			word_list[counter1 - 1] = tempstring;
		}
		else {
			counter1--;
		}
	}

	std::cout << "入力された単語\n";

	for (int i = 1; i <= counter1; i++) {
		std::cout << word_list[i - 1] << " ";
	}

	std::cout << "\n";

	////////////////////////////////////////////////////////////
	//		質問準備
	////////////////////////////////////////////////////////////
	int *numberarray1 = new int[counter1 * counter1 - 1];//組み合わせ番号の１つ目
	int *numberarray2 = new int[counter1 * counter1 - 1];//組み合わせ番号の２つ目

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

	//スコア
	int score1;

	////////////////////////////////////////////////////////////////////////
	//ランダムに組み合わせを表示して関係性を質問する
	////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < counter1 * counter1; i++) {

		int tmpnumber1;//乱数配列から取得した乱数
		tmpnumber1 = randomnumberarray[i];

		score1 = 0;
		wordnumber1 = numberarray1[tmpnumber1];
		wordnumber2 = numberarray2[tmpnumber1];

		if (wordnumber1 != wordnumber2) {
			if (-1 != score_array[wordnumber1][wordnumber2]) {
				system("cls");
				std::cout << "\n     " << word_list[wordnumber1] << "     " << word_list[wordnumber2] << "\n\n";
				std::cout << "  0 : 関係ない\n  1 : 少し関係ある\n  2 : とても関係がある\n\n番号を入力:";
				std::cin >> score1;
				assert(2 >= score1);
				assert(0 <= score1);
			}
		}
		else {
			//同じ単語の場合は交差部のスコアを入れる
			score1 = intersectpoint1;
		}
		score_array[wordnumber1][wordnumber2] = score1;
		score_array[wordnumber2][wordnumber1] = score1;
	}

	//マトリクス出力
	for (int i = 0; i < counter1; i++) {
		for (int j = 0; j < counter1; j++) {
			std::cout << score_array[i][j];
		}
		std::cout << "\n";
	}

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
	std::cout << r_script_fullpath << "\n";

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
	command_execute(batch_path);

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

