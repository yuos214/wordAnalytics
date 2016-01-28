#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <assert.h>
#include <stdlib.h>
#include <direct.h>
#include <Windows.h>

//�v���g�^�C�v�錾
std::string openReadFile();
int createrandomarray(int outputnumberarray[], int n);
std::string create_r_script(void);
std::string replace_text(std::string target, std::string find, std::string replace);

//�萔�ݒ�
const int threshold = 0;																	//���Ƃ̊֌W�����ɒ[�ɔ����P����폜���邽�߂�臒l
const int intersectpoint1 = 3;																//���s���̓��_
const int maxword_count = 10;																//�P��̍ő吔

const std::string tmpdir1 = "temp";															//�ꎞ�t�H���_��
const std::string resultdir1 = "result";													//���ʃt�@�C���̃t�H���_��
const std::string rscript = "Rscript.exe";													//R�X�N���v�g�̎��s�t�@�C����
const std::string scriptname1 = "script.R";													//R�X�N���v�g��
const std::string csvfilename1 = "output.csv";												//�X�R�A�}�g���N�X�̃t�@�C����
const std::string rbatchname1 = "rbat.bat";													//R�X�N���v�g���s�p�o�b�`�̃t�@�C����
const std::string rscriptexe_fullpath = "C:\\Program Files\\R\\R-3.2.3\\bin\\Rscript.exe";	//Rscript.exe�̃t���p�X

int main(void) {

	/////////////////////////////////////////////////////////////
	//			�P�����
	//			�ő�P�O��
	//			"EXIT"���͂œ��͏I��
	/////////////////////////////////////////////////////////////
	int counter1 = 0;
	std::string tempstring = "";	//�P����͗p�ꎞ�ϐ�
	std::string stringarray1[9];	//���͂��ꂽ�P��̔z��

	std::cout << "���������͂��Ă��������i�ő�P�O�j\n�I������ꍇ��\"EXIT\"�����\n";
	while (counter1 <= 10 && tempstring != "EXIT") {

		counter1++;	//���͂��ꂽ��

		std::cout << counter1 << ':';
		std::getline(std::cin, tempstring);

		if (tempstring != "EXIT") {
			stringarray1[counter1 - 1] = tempstring;
		}
		else {
			counter1--;
		}
	}

	std::cout << "���͂��ꂽ�P��\n";

	for (int i = 1; i <= counter1; i++) {
		std::cout << stringarray1[i - 1] << " ";
	}

	std::cout << "\n";

	////////////////////////////////////////////////////////////
	//		���⏀��
	////////////////////////////////////////////////////////////
	int *numberarray1 = new int[counter1 * counter1 - 1];//�g�ݍ��킹�ԍ��̂P��
	int *numberarray2 = new int[counter1 * counter1 - 1];//�g�ݍ��킹�ԍ��̂Q��

														 //���I�ȂQ�����z����쐬
														 //�X�R�A�L�^�p�Q�����z��
	int **scorearray = new int*[counter1 - 1];
	for (int i = 0; i < counter1; i++) {
		scorearray[i] = new int[counter1 - 1];
	}

	int counter2 = 0;
	for (int i = 0; i < counter1; i++) {
		for (int j = 0; j < counter1; j++) {
			//�g�ݍ��킹�ԍ��̔z��
			numberarray1[counter2] = i;
			numberarray2[counter2] = j;
			//�X�R�A��������
			scorearray[i][j] = -1;

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
			if (-1 != scorearray[wordnumber1][wordnumber2]) {
				system("cls");
				std::cout << "\n     " << stringarray1[wordnumber1] << "     " << stringarray1[wordnumber2] << "\n\n";
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
		scorearray[wordnumber1][wordnumber2] = score1;
		scorearray[wordnumber2][wordnumber1] = score1;
	}

	//�}�g���N�X�o��
	for (int i = 0; i < counter1; i++) {
		for (int j = 0; j < counter1; j++) {
			std::cout << scorearray[i][j];
		}
		std::cout << "\n";
	}

	///////////////////////////////////////////////////////////////////////
	//	�X�R�A���v��臒l�ȉ��̒P����폜����
	///////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////
	//	�X�R�A�\��csv�ŏo��
	///////////////////////////////////////////////////////////////////////

	//�ꎞ�t�H���_�쐬
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
	//	R�̃X�N���v�g�t�@�C�����쐬
	///////////////////////////////////////////////////////////////////////
	std::string r_script_fullpath = create_r_script();
	std::cout << r_script_fullpath << "\n";

	///////////////////////////////////////////////////////////////////////
	//	rscript.exe�̏ꏊ��I��
	///////////////////////////////////////////////////////////////////////
	//std::string rscriptexe_fullpath = openReadFile();
	//if ("" == rscriptexe_fullpath){
	//	system("pause");
	//	return 1;	//�L�����Z�����������ꍇ�͏I��
	//}

	//�t�@�C�����m�F
	//std::cout << rscriptexe_fullpath << "\n";

	///////////////////////////////////////////////////////////////////////
	//	R���s�p�̃o�b�`�t�@�C���쐬
	///////////////////////////////////////////////////////////////////////
	std::ofstream out_file2(tmpdir1 + "\\" + rbatchname1);
	out_file2 << "\"" << rscriptexe_fullpath << "\" \"" << r_script_fullpath << "\"";
	out_file2.close();

	///////////////////////////////////////////////////////////////////////
	//	�o�b�`�t�@�C�����s
	///////////////////////////////////////////////////////////////////////

	//���ʃt�H���_�쐬
	_mkdir(resultdir1.c_str());

	std::string batch_command = tmpdir1 + "\\" + rbatchname1;
	std::cout << batch_command.c_str() << "\n";
	system(batch_command.c_str());

	///////////////////////////////////////////////////////////////////////
	//	�㏈��
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
//				���s���e�FR�őΉ����͂��s���Č��ʂ��o�͂���
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



