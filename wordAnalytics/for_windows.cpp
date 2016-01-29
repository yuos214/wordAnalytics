#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <direct.h>

#include "main.h"

//プロトタイプ宣言
std::string replace_text(std::string target, std::string find, std::string replace);

//////////////////////////////////////////////////////////////////
//			rのスクリプトファイルを作成
//				戻り値：スクリプトファイルのフルパス
//
//				スクリプト内容：Rで対応分析を行って結果を出力する
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

///////////////////////////////////////////////////////////////////////
//		ディレクトリを作成する
///////////////////////////////////////////////////////////////////////
int create_directory(std::string path) {
	_mkdir(path.c_str());
	return 0;
}

///////////////////////////////////////////////////////////////////////
//		R実行用バッチファイルを作成する
//			引数：作成するスクリプトのフルパス
//			戻り値：作成したバッチファイルのフルパス
///////////////////////////////////////////////////////////////////////
std::string create_r_batch(std::string r_script_fullpath){
	std::string batch_fullpath = tmpdir1 + sep + rbatchname1;
	
	std::ofstream out_file2(batch_fullpath);
	out_file2 << "\"" << rscriptexe_fullpath << "\" \"" << r_script_fullpath << "\"";
	out_file2.close();

	return batch_fullpath;
}

///////////////////////////////////////////////////////////////////////
//		指定したコマンドを実行する
///////////////////////////////////////////////////////////////////////
int command_execute(std::string command) {
	system(command.c_str());
	return 0;
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


