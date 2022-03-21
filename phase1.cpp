/**
 * @file phase1.cpp
 * @author Zhouliang Yu (zhouliangyu@link.cuhk.edu.cn)
 * @brief
 * @version 0.1
 * @date 2022-02-20
 *
*/

//------------------------------------
#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include "LableTable.h"
#include <sstream>
#include "phase1.h"
using namespace std;

//------------------------------------

/*! Declaration of global variables
 *
 */

int Label_idx = 0;
LableTable LableMap;


//------------------------------------

/*!
 * To Detect the comment sysmbols and empty lines
*/
bool Comment_blank_detector(string line)
{
    std::string copy (line);
    std::string detector;

    istringstream iss(copy);
    iss >> detector;
    bool big_comm = false;
    if (detector.find('#####') != string::npos ){
        return true;
    }
    return detector == "#" || detector == "";

}


void Lable_Detector(ifstream &in_file, int &Label_idx){
    string line;
    string p1;
    string p2;
    bool is_text = true;

    while (!in_file.eof()){
        getline(in_file, line);
        istringstream iss(line);
        iss >> p1 >> p2;

        if (p1 == ".text") is_text = true;
        if (p1.find('.') != string::npos && p1 != ".text") is_text = false;

        //skip the blank, comments and .data fields
        if (!Comment_blank_detector(line) && is_text && line != ".text")
        {
            //find the ":"
            if (p1.find(':') != string::npos) {
                //store the label in the LabelTable
                int pos = p1.find(':');
                string Lable = p1.substr(0, pos);
                LableMap[Lable] = Label_idx;
                continue;
            }

            if (p2.empty())
            {
                continue; 
            } 
            
            p2= "";
            Label_idx += 1;
            
        }
    } 
}

/*!
* @param filename 
* @return a map contains Labels and corresponding addresses 
*/
LableTable phase1(char* filename)
{
    FILE *fp1;
    fp1 = fopen(filename, "r");
    if (fp1 == NULL) {
        printf("Error: Files are not open correctly \n");
    }

    ifstream in_file;
    in_file.open(filename);

    Lable_Detector(in_file, Label_idx);

    fclose(fp1);
    return LableMap;
}


