#pragma once

#include "for_windows.h"

//�萔�ݒ�
const int threshold = 0;																	//���Ƃ̊֌W�����ɒ[�ɔ����P����폜���邽�߂�臒l
const int intersectpoint1 = 3;																//���s���̓��_
const int maxword_count = 10;																//�P��̍ő吔#pragma once
const std::string tmpdir1 = "temp";															//�ꎞ�t�H���_��
const std::string resultdir1 = "result";													//���ʃt�@�C���̃t�H���_��
const std::string csvfilename1 = "output.csv";												//�X�R�A�}�g���N�X�̃t�@�C����

//windows�p�萔
const std::string rscript = "Rscript.exe";													//R�X�N���v�g�̎��s�t�@�C����
const std::string scriptname1 = "script.R";													//R�X�N���v�g��
const std::string rbatchname1 = "rbat.bat";													//R�X�N���v�g���s�p�o�b�`�̃t�@�C����
const std::string rscriptexe_fullpath = "C:\\Program Files\\R\\R-3.2.3\\bin\\Rscript.exe";	//Rscript.exe�̃t���p�X
const std::string sep = "\\";																//��؂蕶��