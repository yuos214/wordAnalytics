#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <direct.h>

#include "main.h"

//�v���g�^�C�v�錾
std::string replace_text(std::string target, std::string find, std::string replace);

//////////////////////////////////////////////////////////////////
//			r�̃X�N���v�g�t�@�C�����쐬
//				�߂�l�F�X�N���v�g�t�@�C���̃t���p�X
//
//				�X�N���v�g���e�FR�őΉ����͂��s���Č��ʂ��o�͂���
//////////////////////////////////////////////////////////////////
std::string create_r_script(void) {

	//�J�����g�t�H���_�擾
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
//		�t�@�C�����J���_�C�A���O�̕\��
//			�߂�l�F�t�@�C���t���p�X
////////////////////////////////////////////////////////////////////////
std::string openReadFile() {
	OPENFILENAME ofn;
	char filename[256];

	filename[0] = '\0';						//������
	memset(&ofn, 0, sizeof(OPENFILENAME));  //�\���̂�0�ŃN���A
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
//		�f�B���N�g�����쐬����
///////////////////////////////////////////////////////////////////////
int create_directory(std::string path) {
	_mkdir(path.c_str());
	return 0;
}

///////////////////////////////////////////////////////////////////////
//		R���s�p�o�b�`�t�@�C�����쐬����
//			�����F�쐬����X�N���v�g�̃t���p�X
//			�߂�l�F�쐬�����o�b�`�t�@�C���̃t���p�X
///////////////////////////////////////////////////////////////////////
std::string create_r_batch(std::string r_script_fullpath){
	std::string batch_fullpath = tmpdir1 + sep + rbatchname1;
	
	std::ofstream out_file2(batch_fullpath);
	out_file2 << "\"" << rscriptexe_fullpath << "\" \"" << r_script_fullpath << "\"";
	out_file2.close();

	return batch_fullpath;
}

///////////////////////////////////////////////////////////////////////
//		�w�肵���R�}���h�����s����
///////////////////////////////////////////////////////////////////////
int command_execute(std::string command) {
	system(command.c_str());
	return 0;
}

//////////////////////////////////////////////////////////////////
//			��������̕����̑S�u��
//				target=�u���Ώە�����
//				find=�u������
//				replace=�u����̕�����
//////////////////////////////////////////////////////////////////
std::string replace_text(std::string target, std::string find, std::string replace) {
	std::string::size_type position = target.find(find);
	while (position != std::string::npos) {
		target.replace(position, find.length(), replace);
		position = target.find(find);
	}
	return target;
}


