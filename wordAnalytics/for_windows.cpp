//#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <direct.h>

#include "main.h"

//�v���g�^�C�v�錾

////////////////////////////////////////////////////////////////////////
//		�J�����g�f�B���N�g���擾
////////////////////////////////////////////////////////////////////////
std::string get_current_directory(void) {
	//�J�����g�t�H���_�擾
	char tmp[255];
	GetCurrentDirectory(255, tmp);
	//std::string current_directory = tmp;

	return tmp;
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

///////////////////////////////////////////////////////////////////////
//		��ʂ��N���A����
///////////////////////////////////////////////////////////////////////
void refresh_console(void) {
	system("cls");
	return;
}