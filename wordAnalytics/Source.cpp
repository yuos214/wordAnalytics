#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <assert.h>
#include <stdlib.h>
#include <direct.h>
#include <Windows.h>

//プロトタイプ宣言
std::string openReadFile();
int createrandomarray(int outputnumberarray[], int n);
std::string create_r_script(void);
std::string replace_text(std::string target, std::string find, std::string replace);

//定数設定
const int threshold = 0;																	//他との関係性が極端に薄い単語を削除するための閾値
const int intersectpoint1 = 3;																//直行部の得点
const int maxword_count = 10;																//単語の最大数

const std::string tmpdir1 = "temp";															//一時フォルダ名
const std::string resultdir1 = "result";													//結果ファイルのフォルダ名
const std::string rscript = "Rscript.exe";													//Rスクリプトの実行ファイル名
const std::string scriptname1 = "script.R";													//Rスクリプト名
const std::string csvfilename1 = "output.csv";												//スコアマトリクスのファイル名
const std::string rbatchname1 = "rbat.bat";													//Rスクリプト実行用バッチのファイル名
const std::string rscriptexe_fullpath = "C:\\Program Files\\R\\R-3.2.3\\bin\\Rscript.exe";	//Rscript.exeのフルパス

int main(void) {

	/////////////////////////////////////////////////////////////
	//			単語入力
	//			最大１０個
	//			"EXIT"入力で入力終了
	/////////////////////////////////////////////////////////////
	int counter1 = 0;
	std::string tempstring = "";	//単語入力用一時変数
	std::string stringarray1[9];	//入力された単語の配列

	std::cout << "文字列を入力してください（最大１０個）\n終了する場合は\"EXIT\"を入力\n";
	while (counter1 <= 10 && tempstring != "EXIT") {

		counter1++;	//入力された個数

		std::cout << counter1 << ':';
		std::getline(std::cin, tempstring);

		if (tempstring != "EXIT") {
			stringarray1[counter1 - 1] = tempstring;
		}
		else {
			counter1--;
		}
	}

	std::cout << "入力された単語\n";

	for (int i = 1; i <= counter1; i++) {
		std::cout << stringarray1[i - 1] << " ";
	}

	std::cout << "\n";

	////////////////////////////////////////////////////////////
	//		質問準備
	////////////////////////////////////////////////////////////
	int *numberarray1 = new int[counter1 * counter1 - 1];//組み合わせ番号の１つ目
	int *numberarray2 = new int[counter1 * counter1 - 1];//組み合わせ番号の２つ目

														 //動的な２次元配列を作成
														 //スコア記録用２次元配列
	int **scorearray = new int*[counter1 - 1];
	for (int i = 0; i < counter1; i++) {
		scorearray[i] = new int[counter1 - 1];
	}

	int counter2 = 0;
	for (int i = 0; i < counter1; i++) {
		for (int j = 0; j < counter1; j++) {
			//組み合わせ番号の配列
			numberarray1[counter2] = i;
			numberarray2[counter2] = j;
			//スコアを初期化
			scorearray[i][j] = -1;

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
			if (-1 != scorearray[wordnumber1][wordnumber2]) {
				system("cls");
				std::cout << "\n     " << stringarray1[wordnumber1] << "     " << stringarray1[wordnumber2] << "\n\n";
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
		scorearray[wordnumber1][wordnumber2] = score1;
		scorearray[wordnumber2][wordnumber1] = score1;
	}

	//マトリクス出力
	for (int i = 0; i < counter1; i++) {
		for (int j = 0; j < counter1; j++) {
			std::cout << scorearray[i][j];
		}
		std::cout << "\n";
	}

	///////////////////////////////////////////////////////////////////////
	//	スコア合計が閾値以下の単語を削除する
	///////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////
	//	スコア表をcsvで出力
	///////////////////////////////////////////////////////////////////////

	//一時フォルダ作成
	_mkdir(tmpdir1.c_str());

	std::ofstream out_file(tmpdir1 + "\\" + csvfilename1);

	for (int i = 0; i < counter1; i++) {
		out_file << "," << stringarray1[i];
	}
	out_file << "\n";
	for (int i = 0; i < counter1; i++) {
		out_file << stringarray1[i];
		for (int j = 0; j < counter1; j++) {
			out_file << "," << scorearray[i][j];
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
	///////////////////////////////////////////////////////////////////////
	//std::string rscriptexe_fullpath = openReadFile();
	//if ("" == rscriptexe_fullpath){
	//	system("pause");
	//	return 1;	//キャンセルを押した場合は終了
	//}

	//ファイル名確認
	//std::cout << rscriptexe_fullpath << "\n";

	///////////////////////////////////////////////////////////////////////
	//	R実行用のバッチファイル作成
	///////////////////////////////////////////////////////////////////////
	std::ofstream out_file2(tmpdir1 + "\\" + rbatchname1);
	out_file2 << "\"" << rscriptexe_fullpath << "\" \"" << r_script_fullpath << "\"";
	out_file2.close();

	///////////////////////////////////////////////////////////////////////
	//	バッチファイル実行
	///////////////////////////////////////////////////////////////////////

	//結果フォルダ作成
	_mkdir(resultdir1.c_str());

	std::string batch_command = tmpdir1 + "\\" + rbatchname1;
	std::cout << batch_command.c_str() << "\n";
	system(batch_command.c_str());

	///////////////////////////////////////////////////////////////////////
	//	後処理
	///////////////////////////////////////////////////////////////////////
	/*
	delete[] randomnumberarray;
	for (int i = 0; i < counter1; i++) {
	delete[] scorearray[i];
	}
	delete[] scorearray;
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
//				実行内容：Rで対応分析を行って結果を出力する
//////////////////////////////////////////////////////////////////
std::string create_r_script(void) {

	//カレントフォルダ取得
	char tmp[255];
	GetCurrentDirectory(255, tmp);
	std::string current_directory = tmp;

	std::string script_directory_path_slash = replace_text(current_directory, "\\", "/") + "/" + tmpdir1;

	std::ofstream out_file(tmpdir1 + "\\" + scriptname1);

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
	out_file << "write.table(F[ , c(1, 2)], \"../" << resultdir1 << "/CorrespondenceAnalysis_result.csv\", sep=\",\",col.names=NA)" << "\n";
	//out_file << "write.table(F[ , c(1, 2)], \"../" << resultdir1 << "/vertical.csv\", sep=\",\",col.names=NA)" << "\n";
	//out_file << "write.table(G[ , c(1, 2)], \"../" << resultdir1 << "/holizontal.csv\", sep=\",\",col.names=NA)" << "\n";
	out_file << "write.table(S.svd$d, \"singular.csv\", sep=\",\")" << "\n";

	out_file.close();

	return current_directory + "\\" + tmpdir1 + "\\" + scriptname1;
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

////////////////////////////////////////////////////////////////////////
//		ファイルを開くダイアログの表示
//			戻り値：ファイルフルパス
////////////////////////////////////////////////////////////////////////
std::string openReadFile() {
	OPENFILENAME ofn;
	char filename[256];

	filename[0] = '\0';						//初期化
	memset(&ofn, 0, sizeof(OPENFILENAME));  //構造体を0でクリア
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = "rscript.exe\0rscript.exe\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = sizeof(filename);
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "exe";

	if (TRUE == GetOpenFileName(&ofn)) {
		return filename;
	}
	else {
		return "";
	}
}



