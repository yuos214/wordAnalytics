#pragma once

#include <iostream>

//�v���g�^�C�v�錾
std::string openReadFile();
std::string create_r_script(void);
int create_directory(std::string path);
std::string create_r_batch(std::string r_script_fullpath);
int command_execute(std::string command);
std::string get_current_directory(void);