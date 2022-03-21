/**
 * @file simulator.cpp
 * @author Zhouliang Yu (zhouliangyu@link.cuhk.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <bits/stdc++.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <bitset>
#include "LableTable.h"
#include "phase1.h"
#include "phase2.h"
#include "simulator.h"
#include <stdlib.h>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <set>
#include <cstdio>
#include <fcntl.h>

using namespace std;


//------------------------------------
FILE *input_file;
FILE *output_file;
std::set<int> checkpoints;
const size_t  max_size = 6 * 1024 * 1024; // 6MB
const size_t text_size = 1 * 1024 * 1024; // 1MB
const uint32_t text_start = 0x400000;
const uint32_t text_end = 0x500000;
const uint32_t static_data_start = 0x500000;
uint32_t dynamic_data_start = 0x500000; //initialize it to start same pos with static_data_start
uint32_t stack_start = 0xA00000;

uint32_t register_size = 34 * 4;
char *real_mem = (char*) malloc(max_size); //memory allocation
vector <int32_t> reg_mem(34, 0); //28: $gp, 29: $sp, 30: $fp, 32: LO, 33: HI, 31: pc

uint32_t pc = 0x400000; // program counter


map<int, vector<string> > instructions; //store the instructions and its type
map<int, vector<string> > data_map;
//map<string, string> Instruction_map;
//map<string, string> Inst_type_map;
//------------------------------------

/**
 * @brief create a data_map, key: line_count, value: a vector of line component in .data region 
 * 
 * @param in_file_data 
 */
void data_mapping(ifstream& in_file_data){
    bool is_data = true;

    int line_count = 0;
    while(!in_file_data.eof()){
        string line;
        getline(in_file_data, line);
        if (line == ".data")
            is_data = true;
        if (line == ".text")
            is_data = false;

        if (!Comment_blank_detector(line) && line != ".text" && line != ".data" && is_data == true)
        {
            //get rid of useless discripter
            string real_line;
            if (line.find('#') != string::npos)
            {
                int pos = line.find('#');
                real_line = line.substr(0, pos);
            }
            else {
                real_line = line;
            }
            vector<string> result;
            istringstream iss(real_line);
            string data_label;
            string data_mode;
            string data;
            iss >> data_label >> data_mode >> data;
            data_label = data_label.substr(0, data_label.length());
            result.push_back(data_label);
            result.push_back(data_mode);
            int data_pos = real_line.rfind(data_mode) + data_mode.length() + 1;
            data = real_line.substr(data_pos, real_line.length());

            if (data[0] == '\"' && data[data.length() - 1] == '\"') { // the data is a string
                data = data.substr(1, data.length() - 2);
                result.push_back(data);
            }else if (data.find(',') != string::npos){ // the data compose of numbers i.e. 1, 2, 3
                char *discard_item = ",\t";
                char c_line[100];
                char *result_str = strtok(strcpy(c_line, data.c_str()), discard_item);
                result.push_back(result_str);
                while (result_str != NULL)
                {
                    result_str = strtok(NULL, discard_item);
                    if (result_str != NULL)
                    {
                        result.push_back(result_str);
                    }
                }

            }

            data_map[line_count] = result;
            line_count += 1;
        }else{
            continue;
        }
    }

}

/**
 * put the machine code into the text region
 * @param in_file_bin
 * @param text_start
 */

 void put_text(ifstream& in_file_bin){
    uint32_t text_add = text_start;
    while (!in_file_bin.eof()){
        string line;
        getline(in_file_bin, line);
        *reinterpret_cast<int32_t*>(memMap(text_add)) = string_to_int_32(line);
        text_add += 4;
    }
 }

 /**
  * @brief transform a binary-like string to 32-bits integer
  * @param line
  * @param bits
  * @return bit_int
  */

 int32_t string_to_int_32(string line){
     bitset<32> bs(line);
     int32_t bit_int;
     bit_int = (int32_t)(bs.to_ulong());
     return bit_int;
}

int16_t string_to_int_16(string line){
    bitset<16> bs(line);
    int16_t bit_int;
    bit_int = (int16_t)(bs.to_ulong());
    return bit_int;
 }


int string_to_int(string line, int bits){
    if (bits == 6){
        bitset<6> bs(line);
        int bit_int;
        bit_int = (int)(bs.to_ulong());
        return bit_int;
    }else if (bits == 5){
        bitset<5> bs(line);
        int bit_int;
        bit_int = (int)(bs.to_ulong());
        return bit_int;
    }else if (bits == 26) {
        bitset<26> bs(line);
        int bit_int;
        bit_int = (int)(bs.to_ulong());
        return bit_int;
    }else{
            cout << "unknown bits";
            return 0;
    }
 }

void init_checkpoints(char* checkpoint_file){
    FILE * fp = fopen(checkpoint_file, "r");
    int tmp, i = 0;
    while(fscanf(fp, "%d", &tmp) != EOF){
        checkpoints.insert(tmp);
    }
}

void checkpoint_memory(int ins_count){
    if (!checkpoints.count(ins_count))
        return;
    std::string name = "memory_" + std::to_string(ins_count) + ".bin";
    FILE * fp = fopen(name.c_str(), "wb");
    fwrite(real_mem, 1, 0x600000,fp);
    fclose(fp);
}

void checkpoint_register(int ins_count){
    if (!checkpoints.count(ins_count))
        return;
    std::string name = "register_" + std::to_string(ins_count) + ".bin";
    FILE * fp = fopen(name.c_str(), "wb");

    fwrite(&reg_mem[0], 4 , 1, fp);
    fwrite(&reg_mem[1], 4 , 1, fp);
    fwrite(&reg_mem[2], 4 , 1, fp);
    fwrite(&reg_mem[3], 4 , 1, fp);
    fwrite(&reg_mem[4], 4 , 1, fp);
    fwrite(&reg_mem[5], 4 , 1, fp);
    fwrite(&reg_mem[6], 4 , 1, fp);
    fwrite(&reg_mem[7], 4 , 1, fp);
    fwrite(&reg_mem[8], 4 , 1, fp);
    fwrite(&reg_mem[9], 4 , 1, fp);
    fwrite(&reg_mem[10], 4 , 1, fp);
    fwrite(&reg_mem[11], 4 , 1, fp);
    fwrite(&reg_mem[12], 4 , 1, fp);
    fwrite(&reg_mem[13], 4 , 1, fp);
    fwrite(&reg_mem[14], 4 , 1, fp);
    fwrite(&reg_mem[15], 4 , 1, fp);
    fwrite(&reg_mem[16], 4 , 1, fp);
    fwrite(&reg_mem[17], 4 , 1, fp);
    fwrite(&reg_mem[18], 4 , 1, fp);
    fwrite(&reg_mem[19], 4 , 1, fp);
    fwrite(&reg_mem[20], 4 , 1, fp);
    fwrite(&reg_mem[21], 4 , 1, fp);
    fwrite(&reg_mem[22], 4 , 1, fp);
    fwrite(&reg_mem[23], 4 , 1, fp);
    fwrite(&reg_mem[24], 4 , 1, fp);
    fwrite(&reg_mem[25], 4 , 1, fp);


}
 /**
  * @brief extend the 16-bits integer to a 32-bits integer
  * in this project we ignore the negtive case.
  * @param before_ext
  * @return after_ext
  */
 int32_t sign_ext (int before_ext){
    int32_t after_ext = before_ext;
    int32_t sign_mask = 1 << 15;
    int32_t sign = before_ext & sign_mask;
    if (sign == sign_mask){
        after_ext = before_ext | (((1 << 16) - 1) << 16);
    }
    return after_ext;
 }

/**
 * @brief key: valid line num, value: instruction
 * 
 * @param in_file_data 
 */
 void instruction_maping(ifstream &in_file_text)
 {
     int line_count = 0; // count the valid line in the .text field

     bool is_text = true;
     int i = 0;
     while (!in_file_text.eof())
     {
         i += 1;
         string line;
         getline(in_file_text, line);
         if (line == ".data")
             is_text = false;
         if (line == ".text")
             is_text = true;

         if (!Comment_blank_detector(line) && line != ".text" && line != ".data" && is_text == true)
         {
             //get rid of useless discripter
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
                 while (result.size() != 5)
                 {
                     result.push_back("nan");
                 }
             }
             instructions[line_count] = result;

             line_count += 1;
         }
         else
         {
             continue;
         }
     }
 }


void R_operation::add(int rs, int rt, int rd){
    int rs_value = reg_mem[rs];
    int rt_value = reg_mem[rt];
    reg_mem[rd] = rs_value + rt_value;
 }

void R_operation::addu (int rs, int rt, int rd){
     reg_mem[rd] = reg_mem[rs] + reg_mem[rt];
 }

void R_operation::_and (int rs, int rt, int rd){
     reg_mem[rd] = reg_mem[rt] & reg_mem[rs];
 }

 void R_operation::_div (int rs, int rt){
     reg_mem[32] = reg_mem[rs] % reg_mem[rt];
     reg_mem[33] = reg_mem[rs] / reg_mem[rt];
 }

 void R_operation::divu(int rs, int rt){
     reg_mem[32] = (unsigned)(reg_mem[rs] % reg_mem[rt]);
     reg_mem[33] = (unsigned)(reg_mem[rs] / reg_mem[rt]);
 }

 void R_operation::jalr (int rs, int rd){
     reg_mem[rd] = pc;
     pc = reg_mem[rs];
 }

 void R_operation::jr (int rs){
    pc = reg_mem[rs];
 }

 void R_operation::mfhi (int rd){
     reg_mem[rd] = reg_mem[33];
 }

 void R_operation::mflo(int rd){
     reg_mem[rd] = reg_mem[32];
 }

 void R_operation::mthi(int rs){
     reg_mem[33] = reg_mem[rs];
 }

void R_operation::mtlo(int rs){
    reg_mem[32] = reg_mem[rs];
}

void R_operation::mult(int rs, int rt){
     int64_t temp = (int64_t)reg_mem[rs] * (int64_t) reg_mem[rt];
     reg_mem[32] = (uint32_t) temp; //lo
     reg_mem[33] = (uint32_t) (temp >> 32);
 }

void R_operation::multu(int rs, int rt){
    int64_t temp = (unsigned )((int64_t)reg_mem[rs] * (int64_t) reg_mem[rt]);
    reg_mem[32] = (unsigned) ((uint32_t) temp); //lo
    reg_mem[33] = (unsigned )(uint32_t) (temp >> 32);
}

void R_operation::nor (int rs, int rt, int rd){
    reg_mem[rd] = ~(reg_mem[rs] | reg_mem[rd]);
 }

 void R_operation::_or(int rs, int rt, int rd){
     reg_mem[rd] = reg_mem[rs] | reg_mem[rt];
 }

 void R_operation::sll(int rd, int rt, int sa){
     reg_mem[rd] = reg_mem[rt] << sa;
 }

 void R_operation::sllv(int rd, int rt, int rs){
     reg_mem[rd] = reg_mem[rt] << reg_mem[rs];
 }

 void R_operation::slt(int rd, int rs, int rt){
     reg_mem[rd] = (reg_mem[rs] < reg_mem[rt]) ? 1 : 0;
 }

void R_operation::sltu(int rd, int rs, int rt){
    reg_mem[rd] = ((unsigned )reg_mem[rs] < (unsigned )reg_mem[rt]) ? 1 : 0;
 }

 void R_operation::sra(int rd, int rt, int sa){
    reg_mem[rd] = (reg_mem[rt] >> sa);
 }

void R_operation::srav(int rd, int rt, int rs){
    reg_mem[rd] = (reg_mem[rt] >> reg_mem[rs]);
}

void R_operation::srl(int rd, int rt, int sa){
    reg_mem[rd] = (reg_mem[rt] >> sa) & ((1 << (32 - sa)) - 1);
}

void R_operation::srlv(int rd, int rt, int rs){
    reg_mem[rd] = (reg_mem[rt] >> reg_mem[rs]) & ((1 << (32 - reg_mem[rs])) - 1);
}

void R_operation::sub(int rd, int rt, int rs){
    reg_mem[rd] = reg_mem[rs] - reg_mem[rt];
}

void R_operation::subu(int rd, int rt, int rs){
    reg_mem[rd] = (unsigned)( reg_mem[rs] - reg_mem[rt]);
}

void R_operation::syscall(){
     int v0 = reg_mem[2];
    switch (v0) {
        case 1: { //print_int, argument: $a0 = integer, number = 4
            fprintf(output_file, "%d", reg_mem[4]);
            break;
        }

        case 4: {//print_string, argument: $a0 = string, number = 4
            fprintf(output_file, "%s", memMap(reg_mem[4]));
            break;

        }

        case 5: //read_int read from .in file directedly
        {
            fscanf(input_file, "%d", &reg_mem[2]);
            break;
        }

        case 8: //read_string, $a0 = buffer, $a1 = length
        {
            char str[10000];
            memset(str, '\0', sizeof(str));
            int buffer_addr = reg_mem[4];
            int maxlen = reg_mem[5];

            fgets(str, maxlen-1, input_file);
            for (int i = 0; i < maxlen - 1; i++)
            {
                if (str[i] == '\0')
                {
                    *memMap(buffer_addr++) = '\n'; // add new line;
                    break;
                }
                *memMap(buffer_addr++) = str[i];
            }
            break;
        }

        case 9: //sbrk, arguments: $a0 = amount, result: address(in $v0)
        {
            reg_mem[2] = dynamic_data_start;
            int32_t offset = reg_mem[4];
            dynamic_data_start += offset;
            break;
        }

        case 10: //exit
        {
            exit(0);
            break;
        }

        case 11: //print_char, argument $a0 = char
        {
            char char_8 = (char)(reg_mem[4] & ((1 << 8) - 1));
            fputc(char_8, output_file);
            break;
        }

        case 12: //read_char
        {
            reg_mem[4] = fgetc(input_file);
            break;
        }

        case 13: //open, arguments: $a0: file descriptor, $a1: the buffer, $a2: the length
            // result: file descriptor (in $a0)
        {
            int fd = reg_mem[4];
            int flag = reg_mem[5];
            int mode = reg_mem[6];
            reg_mem[2] = open(memMap(fd), flag, mode);
            break;
        }

        case 14: //read, args: $a0 = file descriptor, $a1 = buffer, $a2 = length, result: num chars read(in $a0)
        {
            int fd = reg_mem[4];
            int buffer_addr = reg_mem[5];
            int len = reg_mem[6];
            reg_mem[2] = read(fileno(input_file), memMap(buffer_addr), len);
            break;
        }

        case 15: //write
        {
            int fd = reg_mem[4];
            int buffer_addr = reg_mem[5];
            int len = reg_mem[6];
            reg_mem[2] = write(fileno(input_file), memMap(buffer_addr), len);
            break;
        }

        case 16: //close
        {
            int fd = reg_mem[4];
            close(fd);
            break;
        }

        case 17: //exit2, arg: $a0 = result
        {
            int result = reg_mem[4];
            exit(result);
            break;
        }

        default: {
            break;
        }
     }
 }

 void R_operation::_xor(int rd, int rs, int rt){
    reg_mem[rd] = reg_mem[rs] ^ reg_mem[rt];
 }

 void I_operation::addi(int rt, int rs, int16_t imm){
     int32_t imm_v = sign_ext(imm);
    reg_mem[rt] = reg_mem[rs] + imm_v;
 }

void I_operation::addiu(int rt, int rs, int16_t imm){
    int32_t imm_v = sign_ext(imm);
    reg_mem[rt] = (unsigned )reg_mem[rs] + imm_v;
}

void I_operation::andi(int rt, int rs, int16_t imm){
    int32_t imm_v = sign_ext(imm);
    reg_mem[rt] = reg_mem[rs] & imm_v;
 }

 void I_operation::beq(int rs, int rt, int16_t offset){
    int32_t offset_32 = sign_ext(offset);
    offset_32 <<= 2;
    if (reg_mem[rs] == reg_mem[rt]){
        pc += offset_32;
    }
 }

void I_operation::bgez(int rs, int16_t offset){
    int32_t offset_32 = sign_ext(offset);
    offset_32 <<= 2;
    if (reg_mem[rs] >= 0){
        pc += offset_32;
    }
}

void I_operation::bgtz(int rs, int16_t offset){
    int32_t offset_32 = sign_ext(offset);
    offset_32 <<= 2;
    if (reg_mem[rs] > 0){
        pc += offset_32;
    }
}

void I_operation::blez(int rs, int16_t offset){
    int32_t offset_32 = sign_ext(offset);
    offset_32 <<= 2;
    if (reg_mem[rs] <= 0){
        pc += offset_32;
    }
}

void I_operation::bltz(int rs,  int16_t offset){
    int32_t offset_32 = sign_ext(offset);
    offset_32 <<= 2;
    if (reg_mem[rs] < 0){
        pc += offset_32;
    }
}

void I_operation::bne(int rs, int rt, int16_t offset){
    int32_t offset_32 = sign_ext(offset);
    offset_32 <<= 2;
    if (reg_mem[rs] != reg_mem[rt]){
        pc += offset_32;
    }
}

void I_operation::lb(int rs, int rt, int16_t offset){
    int32_t offset_32 = sign_ext(offset);
    int8_t byte_v = *(memMap( reg_mem[rs] + offset_32));
    int8_t mask = (1<<7);
    int sign = (byte_v & mask == mask) ? 1 : 0;
    reg_mem[rt] = (sign == 1) ? (((1 << 24) - 1) << 8) & byte_v : byte_v;

}

void I_operation::lbu(int rs, int rt, int16_t offset){
    int32_t offset_32 = sign_ext(offset);
    int8_t byte_v = *(memMap( reg_mem[rs] + offset_32));
    reg_mem[rt] = byte_v & ((1 << 8)-1 ); //zero-extent
}

void I_operation::lh(int rs, int rt, int16_t offset){
    int32_t offset_32 = sign_ext(offset);
    int16_t half_word_v = *reinterpret_cast<int16_t *>(memMap(reg_mem[rs] + offset_32));
    reg_mem[rt] = sign_ext(half_word_v);
}

void I_operation::lhu(int rs, int rt, int16_t offset){
    int32_t offset_32 = sign_ext(offset);
    int16_t half_word_v = *reinterpret_cast<int16_t *>(memMap(reg_mem[rs] + offset_32));
    reg_mem[rt] = half_word_v & ((1 << 16) - 1); //zero-extent
}

void I_operation::lui(int rt, int16_t imm){
    reg_mem[rt] = imm << 16;
}

void I_operation::lw(int rs, int rt, int16_t offset){
   int num;
   num = ((reg_mem[rs]-0x400000)/4+text_start+offset);
   reg_mem[rt] = real_mem[num];
}

void I_operation::ori(int rs, int rt, int16_t imm){
     reg_mem[rt] = (reg_mem[rs] | imm);
 }

 void I_operation::sb(int rt, int rs, int16_t imm){
     int32_t offset_32 = sign_ext(imm);
     *reinterpret_cast<int8_t *> (memMap(reg_mem[rs] + offset_32)) = reg_mem[rt] & ((1 << 8)-1);
 }

void I_operation::sh(int rt, int rs, int16_t imm){
    int32_t offset_32 = sign_ext(imm);
    *reinterpret_cast<int16_t *> (memMap(reg_mem[rs] + offset_32)) = reg_mem[rt];
}

void I_operation::sw(int rt, int rs, int16_t imm){
     int num;
     int offset = (reg_mem[rs]-0x400000)/4+imm;
     num = text_start + offset;
     real_mem[num] = reg_mem[rt];
 }

void I_operation::lwl(int rt, int rs, int16_t imm){
    int addr = reg_mem[rs] + imm*4;
    int remain = addr % 4;
    int num = 4 - remain;
    uint8_t textST = (uint8_t ) text_start;
    uint8_t  newaddr = (uint8_t ) (addr-0x400000+textST);
    uint8_t RT = (uint8_t) rt;
    for(int i =0; i < num; i++){
        reg_mem[(RT+i)] = real_mem[(newaddr+i)];
    }

}

void I_operation::lwr(int rt, int rs, int16_t imm){
    int addr = reg_mem[rs] + imm*4;
    int remain = addr % 4 + 1;
    int num = 4 - remain;
    uint8_t textST = (uint8_t) text_start;
    uint8_t  newaddr = (uint8_t) (addr-0x400000+textST);
    uint8_t RT = (uint8_t) rt;
    for(int i =0; i < num; i++){
        reg_mem[(RT- i)] = real_mem[(newaddr - i)];
    }
}

void I_operation::swl(int rt, int rs, int16_t imm){
    int addr = reg_mem[rs] + imm*4;
    int remain = addr % 4;
    int num = 4 - remain;
    uint8_t textST = (uint8_t) text_start;
    uint8_t newaddr = (uint8_t) (addr-0x400000+textST);
    uint8_t  RT = (uint8_t ) rt;
    for(int i =0; i < num; i++){
        reg_mem[(newaddr+i)] = reg_mem[(RT+i)] ;
    }

}

void I_operation::swr(int rt, int rs, int16_t imm){
    int addr = reg_mem[rs] + imm*4;
    int remain = addr % 4 + 1;
    uint8_t textST = (uint8_t) text_start;
    uint8_t newaddr = (uint8_t) (addr-0x400000+textST);
    uint8_t  RT = (uint8_t ) rt + 3;
    for(int i =0; i < remain; i++){
        reg_mem[(newaddr- i)] = reg_mem[(RT - i)] ;
    }
}

void I_operation::slti(int rt, int rs, int16_t imm){
    int32_t offset_32 = sign_ext(imm);
    reg_mem[rt] = (reg_mem[rs] < offset_32) ? 1 : 0;
 }

void I_operation::sltiu(int rt, int rs, int16_t imm){
    int32_t offset_32 = sign_ext(imm);
    reg_mem[rt] = ((uint32_t)reg_mem[rs] < (uint32_t)offset_32) ? 1 : 0;
}

void I_operation::xori(int rt, int rs, int16_t imm){
    reg_mem[rt] = reg_mem[rs] ^ imm;
}

void J_operation::jal(int target){
    reg_mem[31] = pc;
    int pc_4 = pc & (((1 << 4) - 1) << 28);
    pc = pc_4 + ((target << 2) & ((1 << 28) - 1));
}

void J_operation::j(int target){
     int32_t target_32 = sign_ext(target);
    int32_t pc_hi4 = pc & (((1 << 4) - 1) << 28);
    pc = pc_hi4 + ((target_32 << 2) & ((1 << 28) - 1));
}


 /**
  * @brief execute each instruction in the .text region
  * @param out_file
  * @param pc
  */
 void exe_inst(uint32_t& pc){
     int i = 0;
     cout << reg_mem[31];
     while(*reinterpret_cast<int32_t *> (memMap(pc))){
         i += 1;
         int32_t machine_code = *reinterpret_cast<int32_t *> (memMap(pc));
         string str_mc_32 = dec2bin(to_string(machine_code), 32);
         string type = type_inst(str_mc_32);
         if (type == "R"){
             int rs = string_to_int(str_mc_32.substr(6, 5), 5);
             int rt = string_to_int(str_mc_32.substr(11, 5), 5);
             int rd = string_to_int(str_mc_32.substr(16, 5), 5);
             int sa = string_to_int(str_mc_32.substr(21, 5), 5);
             int fc = string_to_int(str_mc_32.substr(26, 6), 6);
             pc += 4;

             if (fc == 0x20){ // add
                R_operation::add(rs, rt, rd);
             }else if (fc == 0x21){ // addu
                R_operation::addu(rs, rt, rd);
             }else if (fc == 0x24){ //and
                R_operation::_and(rs, rt, rd);
             }else if (fc == 0x1A){ //div
                R_operation::_div(rs, rt);
             }else if (fc == 0x1B){ //divu
                R_operation::divu(rs, rt);
             }else if (fc == 0x09){ //jalr
                R_operation::jalr(rs, rd);
             }else if (fc == 0x08){ //jr
                 R_operation::jr(rs);
             }else if (fc == 0x10) { //mfhi
                 R_operation::mfhi(rd);
             }else if (fc == 0x12) { //mflo
                R_operation::mflo(rd);
             }else if (fc == 0x11){ //mthi
                R_operation::mthi(rs);
             }else if (fc == 0x13){ //mtlo
                R_operation::mtlo(rs);
             }else if (fc == 0x18){ //mult
                 R_operation::mult(rs, rt);
             }else if (fc == 0x19){ //multu
                 R_operation::multu(rs, rt);
             }else if (fc == 0x27){ //nor
                 R_operation::nor(rs, rt, rd);
             }else if (fc == 0x25){ //or
                 R_operation::_or(rs, rt, rd);
             }else if (fc == 0x00){ //sll
                R_operation::sll(rd, rt, sa);
             }else if (fc == 0x04){ //sllv
                R_operation::sllv(rd, rt, rs);
             }else if (fc == 0x2A){ //slt
                 R_operation::slt(rd, rs, rt);
             }else if (fc == 0x2b){ //sltu
                R_operation::sltu(rd, rs, rt);
             }else if (fc == 0x03){ //sra
                R_operation::sra(rd, rt, sa);
             }else if (fc == 0x07){ //srav
                 R_operation::srav(rd, rt, rs);
             }else if (fc == 0x02){ //srl
                 R_operation::srl(rd, rt, sa);
             }else if (fc == 0x06){ //srlv
                R_operation::srlv(rd, rt, rs);
             }else if (fc == 0x22){ //sub
                 R_operation::sub(rd, rt, rs);
             }else if (fc == 0x23){ //subu
                R_operation::subu(rd, rt, rs);
             }else if (fc == 0x0C){ //syscall
                R_operation::syscall();
             }else if (fc == 0x26){ //xor
                R_operation::_xor(rd, rs, rt);
             }else {
                 cout << "no such opcode"<<endl;
             }

         }else if (type == "I"){
             int op = string_to_int(str_mc_32.substr(0, 6), 6);
             int rs = string_to_int(str_mc_32.substr(6, 5), 5);
             int rt = string_to_int(str_mc_32.substr(11, 5), 5);
             int16_t imm = string_to_int_16(str_mc_32.substr(16, 16));

             pc += 4;
             if (op == 0x08){ //addi
                 I_operation::addi(rt, rs, imm);
             }else if (op == 0x09){ //addiu
                I_operation::addiu(rt, rs, imm);
             }else if (op == 0x0C){ //andi
                I_operation::andi(rt, rs, imm);
             }else if (op == 0x04){ //beq
                I_operation::beq(rs, rt, imm);
             }else if (op == 0x01){
                 if (rt == 1){ //bgez
                    I_operation::bgez(rs, imm);
                 }else{ //bltz
                    I_operation::bltz(rs, imm);
                 }
             }else if (op == 0x07){ //bgtz
                I_operation::bgtz(rs, imm);
             }else if (op == 0x06){ //blez
                I_operation::blez(rs, imm);
             }else if (op == 0x05){ //bne
                  I_operation::bne(rs, rt, imm);
             }else if (op == 0x20){ //lb
                I_operation::lb(rs, rt, imm);
             }else if (op == 0x24){ //lbu
                I_operation::lbu(rs, rt, imm);
             }else if (op == 0x21){ //lh
                I_operation::lh(rs, rt, imm);
             }else if (op == 0x25){ //lhu
                I_operation::lhu(rs, rt, imm);
             }else if (op == 0x0F){ //lui
                I_operation::lui(rt, imm);
             }else if (op == 0x23){ //lw
                I_operation::lw(rs, rt, imm);
             }else if (op == 0x0D){ //ori
                I_operation::ori(rs, rt, imm);
             }else if (op == 0x28){ //sb
                I_operation::sb(rt, rs, imm);
             }else if (op == 0x0A){// slti
                I_operation::slti(rt, rs, imm);
             }else if (op == 0x0B){ //sltiu
                I_operation::sltiu(rt, rs, imm);
             }else if (op == 0x29){ //sh
                I_operation::sh(rt, rs, imm);
             }else if (op == 0x2B){ //sw
                 I_operation::sw(rt, rs, imm);
             }else if (op == 0x0E){//xori
                I_operation::xori(rt, rs, imm);
             }else if (op == 0x22){ //lwl
                I_operation::lwl(rt, rs, imm);
             }else if (op == 0x26){ //lwr
                I_operation::lwr(rt, rs, imm);
             }else if (op == 0x2A){ //swl
                I_operation::swl(rt, rs,imm);
             }else if (op == 0x2E){ //swr
                 I_operation::swr(rt, rs,imm);
             }else{
                 cout << "no such opcode"<<endl;
             }
         }else if (type == "J"){
             int op = string_to_int(str_mc_32.substr(0, 6), 6);
             int target = string_to_int(str_mc_32.substr(6, 26), 26);
             pc += 4;
             if(op == 0x02){//j
                J_operation::j(target);
             }else if (op == 0x03){ //jal
                J_operation::jal(target);
             }else {
                 cout << "no such opcode"<<endl;
             }

         }else{
             cout << "unknown type" << endl;
         }
     }
 }



string type_inst(string str_mc_32){
     string op = str_mc_32.substr(0, 6);
     if (op == "000000"){
        return "R";
     }else if (op == "000010" || op == "000011"){
         return "J";
     }else{
         if (op.substr(0, 4) == "0100"){
             return "unknown type";
         }else{
             return "I";
         }
     }
 }

/**
 * @brief put the data into the memory according
 * @param mode
 * @param data
 * @param dynamic_data_start
 */

void put_data_mode(string mode, vector<string> data){
    if(mode == ".ascii"){
        size_t len = data[0].length();
        for (size_t i = 0; i != len; i++){
            if(data[0][i] == '\\' && i + 1 < len){
                switch(data[0][i+1]){
                    case 'n':
                        *memMap(dynamic_data_start++) = '\n';
                    case 't':
                        *memMap(dynamic_data_start++) = '\t';
                    case '\'':
                        *memMap(dynamic_data_start++) = '\'';
                    case '\\':
                        *memMap(dynamic_data_start++) = '\\';
                    case '\"':
                        *memMap(dynamic_data_start++) = '\"';
                    case 'r':
                        *memMap(dynamic_data_start++) = '\r';
                    default:
                        break;
                }
            }else{
                *memMap(dynamic_data_start++) = data[0][i];
            }
        }
    }else if (mode == ".asciiz"){
        size_t len = data[0].length();
        for (size_t i = 0; i < len; i++){
            if (data[0][i] == '\\' && i + 1 < len){
                switch (data[0][i+1]){
                     case 'n':
                         *memMap(dynamic_data_start++) = '\n';
                     case 't':
                         *memMap(dynamic_data_start++) = '\t';
                     case '\'':
                         *memMap(dynamic_data_start++) = '\'';
                     case '\\':
                         *memMap(dynamic_data_start++) = '\\';
                     case '\"':
                         *memMap(dynamic_data_start++) = '\"';
                     case 'r':
                         *memMap(dynamic_data_start++) = '\r';
                     default:
                         break;
                }
            }else{
                     *memMap(dynamic_data_start++) = data[0][i];
                 }

            if (i == len){
                *memMap(dynamic_data_start++) = '\0'; //add a null
            }
        }
    }else if (mode == ".word"){
        for (auto i:data){
            *reinterpret_cast<int32_t *>(memMap(dynamic_data_start)) = stoi(i);
            dynamic_data_start += 4;
        }
    }else if (mode == ".half"){
        for (auto i:data){
            *reinterpret_cast<int16_t *>(memMap(dynamic_data_start)) = stoi(i);
            dynamic_data_start += 2;
        }
    }else if (mode == ".byte"){
        for (auto i: data){
            *memMap(dynamic_data_start) = stoi(i);
            dynamic_data_start += 1;
        }
    }else{
        cout << "unsupported data type" << endl;
    }

 }


void data::data::putData(string mode, string line)
{

    if (mode == "word")
    {
        if (dynamic_data_start % 4 != 0)
        {
            dynamic_data_start = (dynamic_data_start / 4) + 1;
        } // align
        size_t found = line.find_last_of(".word");
        line = line.substr(found + 1);
        vector<string> v = split(line, "\t\r, ");
        for (auto i : v)
        {
            *reinterpret_cast<int32_t *>(memMap(dynamic_data_start)) = stoi(i);
            dynamic_data_start += 4;
        }
    }

    else if (mode == "ascii")
    {
        size_t found_l = line.find('\"');
        size_t found_r = line.rfind('\"');
        string str = line.substr(found_l + 1, found_r - found_l - 1);


        int len = str.length();
        size_t i = 0;
        for (i; i < len; i++)
        {
            if (str[i] == '\\')
            {
                char nxt = str[i + 1];
                if (nxt == 'n')
                {
                    *memMap(dynamic_data_start++) = '\n';
                }

                else if (nxt == 't')
                {
                    *memMap(dynamic_data_start++) = '\t';
                }

                else if (nxt == '0')
                {
                    *memMap(dynamic_data_start++) = '\0';
                }
                else
                    continue;

                i++;
            }
            else
            {
                *memMap(dynamic_data_start++) = str[i];
            }
        }
    }

    else if (mode == "half")
    {


        size_t found = line.find_last_of(".half");
        line = line.substr(found + 1);
        vector<string> v = split(line, "\t\r, ");
        for (auto i : v)
        {
            *reinterpret_cast<int16_t *>(memMap(dynamic_data_start)) = (int16_t)stoi(i); // 16-bit quantities
            dynamic_data_start += 2;
        }
    }
    else if (mode == "byte")
    {

        size_t found = line.find_last_of(".byte");
        line = line.substr(found + 1);
        vector<string> v = split(line, "\t\r, ");
        for (auto i : v)
        {
            *memMap(dynamic_data_start) = stoi(i); //
            dynamic_data_start += 1;
        }
    }
    else
    {
        cout << "unknwon data type";
    }
}

void data::readData(string filename)
{

    ifstream infile(filename);
    std::string line;

    while (std::getline(infile, line))
    {
        if (line.find(".data") != std::string::npos)
            break;
    }

    while (std::getline(infile, line))
    {
        if (line.find(".text") != string::npos)
            break;
        if (line.find('#') != string::npos)
        {
            line = line.substr(0, line.find('#'));
        }

        if (line.length() == 0)
            continue;

        if (line.find(".word") != std::string::npos)
        {

            data::putData("word", line);
        }

        else if (line.find("asciiz") != std::string::npos)
        {

            data::putData("ascii", line);
            *memMap(dynamic_data_start++) = '\0';
        }

        else if (line.find("ascii") != std::string::npos)
        {
            data::putData("ascii", line);
        }

        else if (line.find("half") != std::string::npos)
        {
            data::putData("half", line);
        }

        else if (line.find("byte") != std::string::npos)
        {
            data::putData("byte", line);
        }

        if (dynamic_data_start % 4 != 0)
            dynamic_data_start = ((dynamic_data_start / 4) + 1) * 4;
    }

    infile.close();
}
 /**
  * maps the real memory to the virtual memory
  * @param sim_addr
  * @return
  */
char *memMap(uint32_t sim_addr){
    return real_mem + sim_addr - text_start;
}

int main(int argc, char *argv[])
{

    if (argc < 6)
    {
        printf("Please enter an in, an output file, and expected output file \n");
    }
    //get a labelTable and an output binary assemble translation

//    LableTable table = phase1(argv[1]);
//    phase2(argv[1], table);

    ifstream in_file_text, in_file_bin;
    ofstream out_file;

    in_file_bin.open(argv[2]); //arg[2]: output file of assemble translation
//    out_file.open(argv[3]);
    in_file_text.open(argv[1]);

//    Inst_mapping(Instruction_map);
//    Inst_type_mapping(Instruction_map);

    instruction_maping(in_file_text); //store the instruction in .text and its line num
    in_file_text.close();

    ifstream in_file_data;
    in_file_data.open(argv[1]);
    data::readData(argv[1]);

    put_text(in_file_bin);
    in_file_bin.close();
    in_file_data.close();
    out_file.close();

    init_checkpoints(argv[3]);
    input_file = fopen(argv[4], "r"); //.in file
    output_file = fopen(argv[5], "w"); //.out file
    reg_mem[28] = 0x508001;
    reg_mem[30] = 0xA00000;
    reg_mem[29] = 0xA00000;
    reg_mem[31] = pc;
    exe_inst(pc); //execute the instructions
    checkpoint_memory(checkpoints.size());
    std::set<int> ::iterator  inst_it;
    for (inst_it = checkpoints.begin(); inst_it != checkpoints.end(); ++inst_it){
        checkpoint_register(*inst_it);
        checkpoint_memory(*inst_it);
    }

    fclose(input_file);
    fclose(output_file);

    free(real_mem);
    return 0;
}
