#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <assert.h>
#include <stdlib.h>

#include "main.h"

//�v���g�^�C�v�錾
int createrandomarray(int outputnumberarray[], int n);
std::string replace_text(std::string target, std::string find, std::string replace);

int main(void) {

	/////////////////////////////////////////////////////////////
	//			�P�����
	//			�ő�P�O��
	//			"EXIT"���͂œ��͏I��
	/////////////////////////////////////////////////////////////
	int counter1 = 0;
	std::string tempstring = "";	//�P����͗p�ꎞ�ϐ�
	std::string word_list[9];		//���͂��ꂽ�P��̔z��

	//���b�Z�[�W�o��
	std::cout << "���t����͂��Ă��������i�ő�P�O�j\n�I������ꍇ��\"EXIT\"�����\n���P�ꂪ�d�����Ȃ��悤�����ӂ�������\n\n";

	//�P����͏���
	while (counter1 <= 10 && tempstring != "EXIT" && tempstring != "exit") {

		counter1++;	//���͂��ꂽ��

		std::cout << counter1 << ':';
		std::getline(std::cin, tempstring);

		if (tempstring != "EXIT" && tempstring != "exit") {
			word_list[counter1 - 1] = tempstring;
		}
		else {
			counter1--;
		}
	}

	//���͂��ꂽ�P�����ʂɏo��
	std::cout << "���͂��ꂽ���t\n";

	for (int i = 1; i <= counter1; i++) {
		std::cout << word_list[i - 1] << " ";
	}

	std::cout << "\n";

	////////////////////////////////////////////////////////////
	//		���⏀��
	////////////////////////////////////////////////////////////
	int *numberarray1 = new int[counter1 * counter1 - 1];	//�g�ݍ��킹�ԍ��̂P��
	int *numberarray2 = new int[counter1 * counter1 - 1];	//�g�ݍ��킹�ԍ��̂Q��

	 //���I�ȂQ�����z����쐬
	 //�X�R�A�L�^�p�Q�����z��
	int **score_array = new int*[counter1 - 1];
	for (int i = 0; i < counter1; i++) {
		score_array[i] = new int[counter1 - 1];
	}

	int counter2 = 0;
	for (int i = 0; i < counter1; i++) {
		for (int j = 0; j < counter1; j++) {
			//�g�ݍ��킹�ԍ��̔z��
			numberarray1[counter2] = i;
			numberarray2[counter2] = j;
			//�X�R�A��������
			score_array[i][j] = -1;

			counter2++;
		}
	}

	//�����z��쐬
	int *randomnumberarray = new int[counter1*counter1 - 1];
	createrandomarray(randomnumberarray, counter1*counter1 - 1);

	//��r�Ɏg�p����P��̔ԍ�
	int wordnumber1;
	int wordnumber2;

	////////////////////////////////////////////////////////////////////////
	//�����_���ɑg�ݍ��킹��\�����Ċ֌W�������₷��
	////////////////////////////////////////////////////////////////////////

	//�X�R�A
	int score1;

	//��������łQ�Â��P���\�����A�֌W������͂��Ă��炤
	for (int i = 0; i < counter1 * counter1; i++) {

		int tmpnumber1;//�����z�񂩂�擾��������
		tmpnumber1 = randomnumberarray[i];

		score1 = 0;
		wordnumber1 = numberarray1[tmpnumber1];
		wordnumber2 = numberarray2[tmpnumber1];

		if (wordnumber1 != wordnumber2) {
			if (-1 != score_array[wordnumber1][wordnumber2]) {
				
				refresh_console();	//��ʂ��N���A����

				//�Q�̒P���\������
				std::cout << "\n     " << word_list[wordnumber1] << "     " << word_list[wordnumber2] << "\n\n";
				std::cout << "  0 : �֌W�Ȃ�\n  1 : �����֌W����\n  2 : �ƂĂ��֌W������\n\n�ԍ������:";

				//���[�U�[�ɐ��l����͂��Ă��炤
				std::cin >> score1;

				//���̓`�F�b�N
				while (2 < score1 || 0 > score1) {
					std::cout << "0,1,2�̂ǂꂩ����͂��Ă�������\n�ԍ������:";
					std::cin >> score1;
				}
			}
		}
		else {
			//�����P��̏ꍇ�͌������̃X�R�A������
			score1 = intersectpoint1;
		}
		//�X�R�A���X�R�A�z��ɓ����
		score_array[wordnumber1][wordnumber2] = score1;
		score_array[wordnumber2][wordnumber1] = score1;
	}

	refresh_console();	//��ʂ��N���A����

	///////////////////////////////////////////////////////////////////////
	//	�X�R�A���v��臒l�ȉ��̒P����폜����
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	//		�f�B���N�g������
	///////////////////////////////////////////////////////////////////////
	//�ꎞ�t�H���_�쐬
	create_directory(tmpdir1);
	//���ʃt�H���_�쐬
	create_directory(resultdir1);

	///////////////////////////////////////////////////////////////////////
	//	�X�R�A�\��csv�ŏo��
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
	//	R�̃X�N���v�g�t�@�C�����쐬
	///////////////////////////////////////////////////////////////////////
	std::string r_script_fullpath = create_r_script();

	///////////////////////////////////////////////////////////////////////
	//	rscript.exe�̏ꏊ��I��
	//	!!!!!!!! �萔�őΉ� !!!!!!!!!
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	//	R���s�p�̃o�b�`�t�@�C���쐬
	///////////////////////////////////////////////////////////////////////
	std::string batch_path = create_r_batch(r_script_fullpath);

	///////////////////////////////////////////////////////////////////////
	//	�o�b�`�t�@�C�����s
	///////////////////////////////////////////////////////////////////////
	std::cout << "�Ή����͊J�n";
	command_execute(batch_path);
	std::cout << "�Ή����͏I��\n";

	///////////////////////////////////////////////////////////////////////
	//	�I���p���b�Z�[�W���o��
	///////////////////////////////////////////////////////////////////////
	std::cout << "-------------------------------------------------------------------------------------\n";
	std::cout << "���͂��ꂽ���t�̍��W�l��\"" << tmpdir1 << sep << correspondence_result_filename << "\"�ɏo�͂��܂���\n";
	std::cout << "Excel���ŎU�z�}���쐬���Ă�������\n";

	///////////////////////////////////////////////////////////////////////
	//	�㏈��
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
//			0�`n�̃����_���ȕ��т̘A�Ԕz����쐬
//				ouputnumberarray = �o�͗p�z��
//				n = �z��̒���
//////////////////////////////////////////////////////////////////
int createrandomarray(int outputnumberarray[], int n) {

	float *randomarray = new float[n];//����������z��

	std::random_device rnd;//�����_���쐬�p

	for (int i = 0; i <= n; i++) {
		randomarray[i] = rnd();
	}

	for (int i = 0; i <= n; i++) {

		float tmpnum = randomarray[i];
		int counter1 = 0;//tmpnum��菬���������̐�

						 //�����_���z��̒��̎����ȉ��̐����̌������ʂƂ���
		for (int j = 0; j <= n; j++) {
			if (randomarray[j] < tmpnum) {
				counter1++;
			}
		}

		//���ʂ��o�͔z��ɓ����
		outputnumberarray[i] = counter1;

	}

	//delete[] randomarray;

	return 0;

}

//////////////////////////////////////////////////////////////////
//			r�̃X�N���v�g�t�@�C�����쐬
//				�߂�l�F�X�N���v�g�t�@�C���̃t���p�X
//
//				�X�N���v�g���e�FR�őΉ����͂��s���Č��ʂ��o�͂���
//////////////////////////////////////////////////////////////////
std::string create_r_script(void) {

	//�J�����g�t�H���_�擾
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
