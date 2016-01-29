#pragma once

#include "for_windows.h"

//定数設定
const int threshold = 0;																	//他との関係性が極端に薄い単語を削除するための閾値
const int intersectpoint1 = 3;																//直行部の得点
const int maxword_count = 10;																//単語の最大数#pragma once
const std::string tmpdir1 = "temp";															//一時フォルダ名
const std::string resultdir1 = "result";													//結果ファイルのフォルダ名
const std::string csvfilename1 = "output.csv";												//スコアマトリクスのファイル名

//windows用定数
const std::string rscript = "Rscript.exe";													//Rスクリプトの実行ファイル名
const std::string scriptname1 = "script.R";													//Rスクリプト名
const std::string rbatchname1 = "rbat.bat";													//Rスクリプト実行用バッチのファイル名
const std::string rscriptexe_fullpath = "C:\\Program Files\\R\\R-3.2.3\\bin\\Rscript.exe";	//Rscript.exeのフルパス
const std::string sep = "\\";																//区切り文字