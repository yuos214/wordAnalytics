//#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <direct.h>

#include "main.h"

//プロトタイプ宣言

////////////////////////////////////////////////////////////////////////
//		カレントディレクトリ取得
////////////////////////////////////////////////////////////////////////
std::string get_current_directory(void) {
	//カレントフォルダ取得
	char tmp[255];
	GetCurrentDirectory(255, tmp);
	//std::string current_directory = tmp;

	return tmp;
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

///////////////////////////////////////////////////////////////////////
//		画面をクリアする
///////////////////////////////////////////////////////////////////////
void refresh_console(void) {
	system("cls");
	return;
}