/**
 * @file phase2.h
 * @author Zhouliang Yu (zhouliangyu@link.cuhk.edu.cn)
 * @brief
 * @version 0.1
 * @date 2022-02-20
 *
 */

//------------------------------------
#include <bits/stdc++.h>
#include "LableTable.h"
using namespace std;
//------------------------------------

LableTable phase2(char *filename, LableTable table);
void Inst_mapping(map<string, string> &Instruction_map);
void reg_mapping(map<string, string> &Reg_map);
void Inst_type_mapping(map<string, string> &Instruction_map);
string assembler(int &Label_idx, string line, LableTable &table);
void translate(ifstream &in_file, ofstream &out_file, LableTable &table);
string dec2bin(string dec, int bits);
vector<string> filter(string line);
vector<string> split(const string &str, const string &delim);