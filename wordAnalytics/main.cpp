#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <assert.h>
#include <stdlib.h>

#include "main.h"

//�v���g�^�C�v�錾
int createrandomarray(int outputnumberarray[], int n);

int main(void) {

	/////////////////////////////////////////////////////////////
	//			�P�����
	//			�ő�P�O��
	//			"EXIT"���͂œ��͏I��
	/////////////////////////////////////////////////////////////
	int counter1 = 0;
	std::string tempstring = "";	//�P����͗p�ꎞ�ϐ�
	std::string word_list[9];	//���͂��ꂽ�P��̔z��

	std::cout << "���������͂��Ă��������i�ő�P�O�j\n�I������ꍇ��\"EXIT\"�����\n";
	while (counter1 <= 10 && tempstring != "EXIT") {

		counter1++;	//���͂��ꂽ��

		std::cout << counter1 << ':';
		std::getline(std::cin, tempstring);

		if (tempstring != "EXIT") {
			word_list[counter1 - 1] = tempstring;
		}
		else {
			counter1--;
		}
	}

	std::cout << "���͂��ꂽ�P��\n";

	for (int i = 1; i <= counter1; i++) {
		std::cout << word_list[i - 1] << " ";
	}

	std::cout << "\n";

	////////////////////////////////////////////////////////////
	//		���⏀��
	////////////////////////////////////////////////////////////
	int *numberarray1 = new int[counter1 * counter1 - 1];//�g�ݍ��킹�ԍ��̂P��
	int *numberarray2 = new int[counter1 * counter1 - 1];//�g�ݍ��킹�ԍ��̂Q��

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

	//�X�R�A
	int score1;

	////////////////////////////////////////////////////////////////////////
	//�����_���ɑg�ݍ��킹��\�����Ċ֌W�������₷��
	////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < counter1 * counter1; i++) {

		int tmpnumber1;//�����z�񂩂�擾��������
		tmpnumber1 = randomnumberarray[i];

		score1 = 0;
		wordnumber1 = numberarray1[tmpnumber1];
		wordnumber2 = numberarray2[tmpnumber1];

		if (wordnumber1 != wordnumber2) {
			if (-1 != score_array[wordnumber1][wordnumber2]) {
				system("cls");
				std::cout << "\n     " << word_list[wordnumber1] << "     " << word_list[wordnumber2] << "\n\n";
				std::cout << "  0 : �֌W�Ȃ�\n  1 : �����֌W����\n  2 : �ƂĂ��֌W������\n\n�ԍ������:";
				std::cin >> score1;
				assert(2 >= score1);
				assert(0 <= score1);
			}
		}
		else {
			//�����P��̏ꍇ�͌������̃X�R�A������
			score1 = intersectpoint1;
		}
		score_array[wordnumber1][wordnumber2] = score1;
		score_array[wordnumber2][wordnumber1] = score1;
	}

	//�}�g���N�X�o��
	for (int i = 0; i < counter1; i++) {
		for (int j = 0; j < counter1; j++) {
			std::cout << score_array[i][j];
		}
		std::cout << "\n";
	}

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
	std::cout << r_script_fullpath << "\n";

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
	command_execute(batch_path);

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

