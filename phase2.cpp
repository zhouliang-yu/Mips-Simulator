/**
 * @file phase2.cpp
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
#include "phase2.h"
#include "phase1.h"
using namespace std;

//------------------------------------

/*! Declaration of global variables
 *
 */
map<string, string> Instruction_map;
map<string, string> Reg_map;
map<string, string> Inst_type_map;
map<string, string> extraction_order;
//------------------------------------

void Inst_mapping (map<string, string> &Instruction_map){
    // R-instruction
    // R-type
    Instruction_map["add"] = "000000ssssstttttddddd00000100000";
    Instruction_map["addu"] = "000000ssssstttttddddd00000100001";
    Instruction_map["sub"] = "000000ssssstttttddddd00000100010";
    Instruction_map["subu"] = "000000ssssstttttddddd00000100011";
    Instruction_map["and"] = "000000ssssstttttddddd00000100100";
    Instruction_map["or"] = "000000ssssstttttddddd00000100101";
    Instruction_map["xor"] = "000000ssssstttttddddd00000100110";
    Instruction_map["nor"] = "000000ssssstttttddddd00000100111";
    Instruction_map["div"] = "000000sssssttttt0000000000011010";
    Instruction_map["divu"] = "000000sssssttttt0000000000011011";
    Instruction_map["slt"] = "000000ssssstttttddddd00000101010";
    Instruction_map["sltu"] = "000000ssssstttttddddd00000101011";
    Instruction_map["sll"] = "00000000000tttttdddddvvvvv000000";
    Instruction_map["srl"] = "00000000000tttttdddddvvvvv000010";
    Instruction_map["jr"] = "000000sssss000000000000000001000";
    Instruction_map["jalr"] = "000000sssss000000dddddd000001001";
    Instruction_map["mfhi"] = "0000000000000000ddddd00000011100";
    Instruction_map["mflo"] = "0000000000000000ddddd00000011010";
    Instruction_map["mult"] = "000000sssssttttt0000000000011000";
    Instruction_map["mthi"] = "000000ddddd000000000000000010001";
    Instruction_map["mtlo"] = "000000ddddd000000000000000010011";
    Instruction_map["multu"] = "000000sssssttttt0000000000011001";
    Instruction_map["sllv"] = "000000ssssstttttddddd00000000100";
    Instruction_map["sra"] = "00000000000tttttdddddvvvvv000011";
    Instruction_map["srav"] = "000000ssssstttttddddd00000000111";
    Instruction_map["srlv"] = "000000ssssstttttddddd00000000110";
    Instruction_map["syscall"] = "00000000000000000000000000001100";

    // I-type
    Instruction_map["addi"] = "001000ssssstttttiiiiiiiiiiiiiiii";
    Instruction_map["addiu"] = "001001ssssstttttiiiiiiiiiiiiiiii";
    Instruction_map["andi"] = "001100ssssstttttiiiiiiiiiiiiiiii";
    Instruction_map["ori"] = "001101ssssstttttiiiiiiiiiiiiiiii";
    Instruction_map["slti"] = "001010ssssstttttiiiiiiiiiiiiiiii";
    Instruction_map["sltiu"] = "001011ssssstttttiiiiiiiiiiiiiiii";
    Instruction_map["beq"] = "000100ssssstttttllllllllllllllll";
    Instruction_map["bne"] = "000101ssssstttttllllllllllllllll";
    Instruction_map["bgez"] = "000001sssss00001llllllllllllllll";
    Instruction_map["lb"] = "100000ssssstttttoooooooooooooooo";
    Instruction_map["lbu"] = "100100ssssstttttoooooooooooooooo";
    Instruction_map["lwl"] = "100010ssssstttttiiiiiiiiiiiiiiii";
    Instruction_map["lwr"] = "100110ssssstttttiiiiiiiiiiiiiiii";
    Instruction_map["bltz"] = "000001sssss00000llllllllllllllll";
    Instruction_map["lw"] = "100011ssssstttttoooooooooooooooo";
    Instruction_map["sw"] = "101011ssssstttttoooooooooooooooo";
    Instruction_map["lui"] = "00111100000tttttiiiiiiiiiiiiiiii";
    Instruction_map["xori"] = "001110ssssstttttiiiiiiiiiiiiiiii";
    Instruction_map["bgtz"] = "000111sssss00000llllllllllllllll";
    Instruction_map["blez"] = "000110sssss00000llllllllllllllll";
    Instruction_map["lh"] = "100001ssssstttttoooooooooooooooo";
    Instruction_map["lhu"] = "100101ssssstttttoooooooooooooooo";
    Instruction_map["sb"] = "101000ssssstttttoooooooooooooooo";
    Instruction_map["sh"] = "101001ssssstttttoooooooooooooooo";
    Instruction_map["swr"] = "101110ssssstttttiiiiiiiiiiiiiiii";
    Instruction_map["swl"] = "101010ssssstttttiiiiiiiiiiiiiiii";

    // J-type
    Instruction_map["j"] = "000010jjjjjjjjjjjjjjjjjjjjjjjjjj";
    Instruction_map["jal"] = "000011jjjjjjjjjjjjjjjjjjjjjjjjjj";
}

void Inst_type_mapping(map<string, string> &Instruction_map)
{
    for (map<string, string>::iterator it = Instruction_map.begin(); it != Instruction_map.end(); ++it){
        string inst = it -> first;
        string code = it -> second;
        string op = code.substr(0, 6);
        if (op == "000000"){
            Inst_type_map[inst] = "R";
        }else if (op == "000011" || op == "000010"){
            Inst_type_map[inst] = "J";
        }else {
            if (op.substr(0, 4) != "0100"){
                Inst_type_map[inst] = "I";
            }else {
                Inst_type_map[inst] = "Unknown";
            }
        }

    }

}

void reg_mapping (map<string, string> &Reg_map){
    Reg_map["$zero"] = "00000";
    Reg_map["$v0"] = "00010";
    Reg_map["$v1"] = "00011";
    Reg_map["$a0"] = "00100";
    Reg_map["$a1"] = "00101";
    Reg_map["$a2"] = "00110";
    Reg_map["$t0"] = "01000";
    Reg_map["$t1"] = "01001";
    Reg_map["$t2"] = "01010";
    Reg_map["$t3"] = "01011";
    Reg_map["$t4"] = "01100";
    Reg_map["$t5"] = "01101";
    Reg_map["$t6"] = "01110";
    Reg_map["$t7"] = "01111";
    Reg_map["$s0"] = "10000";
    Reg_map["$s1"] = "10001";
    Reg_map["$s2"] = "10010";
    Reg_map["$s3"] = "10011";
    Reg_map["$s4"] = "10100";
    Reg_map["$s5"] = "10101";
    Reg_map["$s6"] = "10110";
    Reg_map["$s7"] = "10111";
    Reg_map["$t8"] = "11000";
    Reg_map["$t9"] = "11001";
    Reg_map["$k0"] = "11010";
    Reg_map["$k1"] = "11011";
    Reg_map["$gp"] = "11100";
    Reg_map["$sp"] = "11101";
    Reg_map["$fp"] = "11110";
    Reg_map["$ra"] = "11111";
}

void extraction_order_mapping(map<string, string> & extraction_order){
    // R-instruction
    // R-type
    extraction_order["add"] = "dst";
    extraction_order["addu"] = "dst";
    extraction_order["sub"] = "dst";
    extraction_order["subu"] = "dst";
    extraction_order["and"] = "dst";
    extraction_order["or"] = "dst";
    extraction_order["xor"] = "dst";
    extraction_order["nor"] = "dst";
    extraction_order["div"] = "st";
    extraction_order["divu"] = "st";
    extraction_order["slt"] = "dst";
    extraction_order["sltu"] = "dst";
    extraction_order["sll"] = "dtv";
    extraction_order["srl"] = "dtv";
    extraction_order["jr"] = "s";
    extraction_order["jalr"] = "ds";
    extraction_order["mfhi"] = "d";
    extraction_order["mflo"] = "d";
    extraction_order["mult"] = "st";
    extraction_order["mthi"] = "s";
    extraction_order["mtlo"] = "s";
    extraction_order["multu"] = "st";
    extraction_order["sllv"] = "dts";
    extraction_order["sra"] = "dtv";
    extraction_order["srav"] = "dtv";
    extraction_order["srlv"] = "dts";
    extraction_order["syscall"] = "";

    // I-type
    extraction_order["addi"] = "tsi";
    extraction_order["addiu"] = "tsi";
    extraction_order["andi"] = "tsi";
    extraction_order["ori"] = "tsi";
    extraction_order["slti"] = "tsi";
    extraction_order["sltiu"] = "tsi";
    extraction_order["beq"] = "stl";
    extraction_order["bne"] = "stl";
    extraction_order["bgez"] = "sl";
    extraction_order["lb"] = "tso";
    extraction_order["lbu"] = "tso";
    extraction_order["lwl"] = "tsi";
    extraction_order["lwr"] = "tsi";
    extraction_order["bltz"] = "sl";
    extraction_order["lw"] = "tso";
    extraction_order["sw"] = "tso";
    extraction_order["lui"] = "ti";
    extraction_order["xori"] = "tsi";
    extraction_order["bgtz"] = "sl";
    extraction_order["blez"] = "sl";
    extraction_order["lh"] = "tso";
    extraction_order["lhu"] = "tso";
    extraction_order["sb"] = "tso";
    extraction_order["sh"] = "tso";
    extraction_order["swr"] = "tsi";
    extraction_order["swl"] = "tsi";

    // J-type
    extraction_order["j"] = "j";
    extraction_order["jal"] = "j";
}


/**
 * @brief when the line is not comment and blank we use assemble to translate the line 
 *              base on the type of instruction
 * @param Label_idx 
 * @param line 
 * @return string 
 */

string assembler(int &Label_idx, string line, LableTable &table)
{
    vector<string> line_par;
    line_par = filter(line);
    string p0 = line_par[0];
    string p1 = line_par[1];
    string p2 = line_par[2];
    string p3 = line_par[3];
    string p4 = line_par[4];

    string result;
    string order;
    
    if (p1 == "nan"){
        return "nai";
    }

    
        
    result = Instruction_map[p0];
    order = extraction_order[p0];
    
    // determine the Type of instruction
    string type = Inst_type_map[p0];
    if (type == "R")
    { // R type instruction
        // if sa != 00000
        if (order == "dtv")
        {
            // replace rd, rt, sa
            string bin_rd = Reg_map[p1];
            string bin_rt = Reg_map[p2];
            int rd_pos = result.find('ddd');
            result.replace(rd_pos, 5, bin_rd);
            int rt_pos = result.find('ttt');
            result.replace(rt_pos, 5, bin_rt);
            string bin_sa = dec2bin(p4, 5);
            int sa_pos = result.find('vvv');
            result.replace(sa_pos, 5, bin_sa);
            return result;
        }
        else
        {
            // sa == "00000"
            if (order == "dts")
            {
                string bin_rd = Reg_map[p1];
                string bin_rt = Reg_map[p2];
                string bin_rs = Reg_map[p3];
                int rd_pos = result.find('ddd');
                result.replace(rd_pos, 5, bin_rd);
                int rt_pos = result.find('ttt');
                result.replace(rt_pos, 5, bin_rt);
                int rs_pos = result.find('sss');
                result.replace(rs_pos, 5, bin_rs);
                return result;
            }
            else if (order == "st")
            {
                string bin_rs = Reg_map[p1];
                string bin_rt = Reg_map[p2];
                int rt_pos = result.find('ttt');
                result.replace(rt_pos, 5, bin_rt);
                int rs_pos = result.find('sss');
                result.replace(rs_pos, 5, bin_rs);
                return result;
            }
            else if (order == "s")
            {
                string bin_rs = Reg_map[p1];
                int rs_pos = result.find('sss');
                result.replace(rs_pos, 5, bin_rs);
                return result;
            }
            else if (order == "ds")
            {
                string bin_rd = Reg_map[p1];
                string bin_rs = Reg_map[p2];
                int rd_pos = result.find('ddd');
                result.replace(rd_pos, 5, bin_rd);
                int rs_pos = result.find('sss');
                result.replace(rs_pos, 5, bin_rs);
                return result;
            }
            else if (order == "d")
            {
                string bin_rd = Reg_map[p1];
                int rd_pos = result.find('ddd');
                result.replace(rd_pos, 5, bin_rd);
            }
            else if (order == "dst")
            {
                string bin_rd = Reg_map[p1];
                string bin_rs = Reg_map[p2];
                string bin_rt = Reg_map[p3];
                int rd_pos = result.find('ddd');
                result.replace(rd_pos, 5, bin_rd);
                int rt_pos = result.find('ttt');
                result.replace(rt_pos, 5, bin_rt);
                int rs_pos = result.find('sss');
                result.replace(rs_pos, 5, bin_rs);
                return result;
            }
            else
            {
                cout << "order:" << order << endl;
            }
        }
    }
    else if (type == "I")
    {
        
        if (order == "tsi")
        {
            string bin_rt;
            string bin_rs;
            string imm;
            if(p3.find('$') != string::npos){
                bin_rt = Reg_map[p1];
                bin_rs = Reg_map[p3];
                imm = p2;
            }else{
                bin_rt = Reg_map[p1];
                bin_rs = Reg_map[p2];
                imm = p3;
            }
            int rt_pos = result.find('ttt');
            result.replace(rt_pos, 5, bin_rt);
            int rs_pos = result.find('sss');
            result.replace(rs_pos, 5, bin_rs);
            int imm_pos = result.find('ii');
            result.replace(imm_pos, 16, dec2bin(imm, 16));
            return result;
        }
        else if (order == "stl")
        {
            string bin_rs = Reg_map[p1];
            string bin_rt = Reg_map[p2];
            string lable = p3;
            int rt_pos = result.find('ttt');
            result.replace(rt_pos, 5, bin_rt);
            int rs_pos = result.find('sss');
            result.replace(rs_pos, 5, bin_rs);
            int label_pos = result.find('lll');
            if (table[lable] < Label_idx)
            {
                // jump the the former line
                int gap = -(Label_idx - table[lable]) - 1;
                result.replace(label_pos, 16, dec2bin(std::to_string(gap), 16));
            }
            else
            {
                int gap = table[lable] - Label_idx - 1;
                result.replace(label_pos, 16, dec2bin(std::to_string(gap), 16));
            }
            return result;
        }
        else if (order == "sl")
        {
            string bin_rs = Reg_map[p1];
            string lable = p2;
            int rs_pos = result.find('sss');
            result.replace(rs_pos, 5, bin_rs);
            int label_pos = result.find('lll');
            if (table[lable] < Label_idx)
            {
                // jump the the former line
                int gap = - (Label_idx - table[lable]) - 1;
                result.replace(label_pos, 16, dec2bin(std::to_string(gap), 16));
            }
            else
            {
                int gap = table[lable] - Label_idx - 1;
                result.replace(label_pos, 16, dec2bin(std::to_string(gap), 16));
            }
            return result;
        }
        else if (order == "ti")
        {
            string bin_rt = Reg_map[p1];
            string imm = p2;
            int rt_pos = result.find('ttt');
            result.replace(rt_pos, 5, bin_rt);
            int imm_pos = result.find('ii');
            result.replace(imm_pos, 16, dec2bin(imm, 16));
            return result;
        }
        else if (order == "tso")
        {
            string bin_rt = Reg_map[p1];
            string bin_rs = Reg_map[p3];
            string imm = p2;
            int rs_pos = result.find('sss');
            result.replace(rs_pos, 5, bin_rs);
            int rt_pos = result.find('ttt');
            result.replace(rt_pos, 5, bin_rt);
            int imm_pos = result.find('ooo');
            result.replace(imm_pos, 16, dec2bin(imm, 16));
            return result;
        }
    }
    else if (type == "J")
    {
        string lable = p1;
        int j_pos = result.find('jjj');
        result.replace(j_pos, 26, dec2bin(std::to_string(1048576 + table[lable]), 26));
        return result;
    }
}

/**
 * @brief converting a decimal number to a binary number
 *  
 * @param dec 
 * @return string 
 */
string dec2bin(string dec, int bits)
{

    int binaryNumber[bits];
    int decimalNumber = atoi(dec.c_str());
    string bin_result = "";
    int add1 = 1;

    if (decimalNumber > 0)
    {
        int i = 0;

        while (decimalNumber > 0)
        {
            binaryNumber[i] = decimalNumber % 2;
            decimalNumber = decimalNumber / 2;
            i++;
        }

        for (int k = i; k != bits; k++)
        {
                binaryNumber[k] = 0;
        }

        for (int j = 0; j <= bits - 1; j++)
        {
            bin_result += std::to_string(binaryNumber[j]);
        }

        reverse(bin_result.begin(), bin_result.end());

        return bin_result;
    }
    else if (decimalNumber < 0)
    {
        // for a negative decimal number, calculate its 2's complement
        decimalNumber = -decimalNumber;
        int i = 0;

        while (decimalNumber > 0)
        {
            binaryNumber[i] = decimalNumber % 2;
            decimalNumber = decimalNumber / 2;
            i++;
        }

        for (int k = i; k != bits; k++)
        {
                binaryNumber[k] = 0;
        }

        int temp = 0;
        // reverse the binaryNumber
        for (int a = 0; a < bits / 2; ++a)
        {
            temp = binaryNumber[bits - a - 1];
            binaryNumber[bits - a - 1] = binaryNumber[a];
            binaryNumber[a] = temp;
        }

        for (int b = 0; b != bits; b++)
        {
            if (b == 0)
            {
                binaryNumber[b] = 1;
            }
            else
            {
                if (binaryNumber[b] == 0)
                {
                    binaryNumber[b] = 1;
                }
                else
                {
                    binaryNumber[b] = 0;
                }
            }
        }

        for (int c = bits - 1; c >= 1; c--)
        {
            if (binaryNumber[c] == 1)
            {
                if (add1 == 1)
                {
                    binaryNumber[c] = 0;
                    add1 = 1;
                }
                else
                {
                    binaryNumber[c] = 1;
                    add1 = 0;
                }
            }
            else
            {
                if (add1 == 1)
                {
                    binaryNumber[c] = 1;
                    add1 = 0;
                }
                else
                {
                    binaryNumber[c] = 0;
                    add1 = 0;
                }
            }
        }

        for (int j = 0; j <= bits - 1; j++)
        {
            bin_result += std::to_string(binaryNumber[j]);
        }

        return bin_result;
    }
    else
    {
        // decimal == 0
        for (int b = 0; b != bits; b++)
        {
            binaryNumber[b] = 0;
        }

        for (int j = 0; j <= bits - 1; j++)
        {
            bin_result += std::to_string(binaryNumber[j]);
        }
    }
}

/**
 * @brief Controling the process of assembler translating
 * 
 * @param in_file 
 * @param out_file 
 */

void translate(int &Label_idx, ifstream &in_file, ofstream &out_file, LableTable &table)
{
    bool is_text = true;
    while(!in_file.eof()){
        string line;
        string result;
        getline(in_file, line);
        if (line == ".data") is_text = false;
        if (line == ".text") is_text = true;

        if (!Comment_blank_detector(line) && line != ".text" && line!=".data" && is_text == true){
            // line_num += 1;
            result = assembler(Label_idx, line, table);
            if(result == "nai") {
                continue;
            }
            out_file << result << endl;
            Label_idx += 1;
        }else{
            continue;
        }
    }

}

/**
 * @brief discard out "," "#" blank () 
 * @return a vector of string that contains each partitions of a line
 * 
 * @param line 
 */
vector<string> filter(string line){
    //remove "comments"
    string real_line;
    if (line.find('#') != string::npos)
    {
        int pos = line.find('#');
        real_line = line.substr(0, pos);
    }
    else
    {
        real_line = line;
    }

    vector<string> result;
    char *discard_item = " :\t\n#,()";
    char c_line[100];
    char *result_str = strtok(strcpy(c_line, real_line.c_str()), discard_item);

    result.push_back(result_str);
    while (result_str != NULL)
    {
        result_str = strtok(NULL, discard_item);
        if (result_str != NULL)
        {
            result.push_back(result_str);
        }
    }
    
    if (result.size() < 5)
    {
        while (result.size() != 5) {
            result.push_back("nan");
        }
    }

    return result;

}

vector<string> split(const string &str, const string &delim)
{
    vector<string> res;
    if ("" == str)
        return res;

    char *strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());

    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while (p)
    {
        string s = p;
        res.push_back(s);
        p = strtok(NULL, d);
    }

    return res;
}

LableTable phase2(char *filename, LableTable table){
    
    //second term scan
    FILE *fp2;
    fp2 = fopen(filename, "r");
    if (fp2 == NULL)
    {
        printf("Error: Files are not open correctly \n");
    }

    ifstream in_file;
    ofstream out_file;
    string line;
    string result_line;

    in_file.open(filename);

    out_file.open("output.txt");
    
    int Label_idx = 0;

    // setting the Instruction and Register mapping relationship
    Inst_mapping(Instruction_map);
    Inst_type_mapping(Instruction_map);
    reg_mapping(Reg_map);
    extraction_order_mapping(extraction_order);
    
    // //start assembler translating 
    translate(Label_idx, in_file, out_file, table);

    fclose(fp2);
    out_file.close();
    return table;

}
