/**
 * @file phase1.h
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
void Lable_Detector(ifstream & in_file, int & count);
bool Comment_blank_detector(string line);
LableTable phase1(char* filename);
