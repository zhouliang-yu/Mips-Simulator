/**
 * @file simulator.h
 * @author Zhouliang Yu (120040077@link.cuhk.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iostream>
#include <string.h>
#include <fstream>
#include <bitset>
#include "LableTable.h"
#include "phase1.h"
#include "phase2.h"
#include <stdlib.h>
using namespace std;

void data_mapping(ifstream &in_file_data);
void exe_inst(uint32_t& pc);
void put_text(ifstream &in_file_bin);
void instruction_maping(ifstream &in_file_data);

char *memMap(uint32_t sim_addr);
//void readData(string filename);
//void putData(string mode, string line);
void put_data();
int32_t string_to_int_32(string line);
int string_to_int(string line, int bits);
string type_inst(string str_mc_32);
void init_checkpoints(char* checkpoint_file);
void checkpoint_memory(int ins_count);
void checkpoint_register(int ins_count);

class data{
public:
    static void readData(string filename);
    static void putData(string mode, string line);
};

class R_operation{
public:
        static void add(int rs, int rt, int rd);
        static void addu(int rs, int rt, int rd);
        static void _and (int rs, int rt, int rd);
        static void _div (int rs, int rt);
        static void divu(int rs, int rt);
        static void jalr (int rs, int rd);
        static void jr (int rs);
        static void mfhi (int rd);
        static void mflo (int rd);
        static void mult(int rs, int rt);
        static void multu(int rs, int rt);
        static void nor (int rs, int rt, int rd);
        static void _or(int rs, int rt, int rd);
        static void sll(int rd, int rt, int sa);
        static void sllv(int rd, int rt, int rs);
        static void slt(int rd, int rs, int rt);
        static void sltu(int rd, int rs, int rt);
        static void sra(int rd, int rt, int sa);
        static void srav(int rd, int rt, int rs);
        static void srl(int rd, int rt, int sa);
        static void srlv(int rd, int rt, int rs);
        static void sub(int rd, int rt, int rs);
        static void subu(int rd, int rt, int rs);
        static void _xor(int rd, int rs, int rt);
        static void mthi(int rs);
        static void mtlo(int rs);

    static void syscall();
};


class I_operation{
public:
        static void addi(int rt, int rs, int16_t imm);
        static void addiu(int rt, int rs, int16_t imm);
        static void andi(int rt, int rs, int16_t imm);
        static void beq(int rs, int rt, int16_t offset);
        static void bgez(int rs, int16_t offset);
        static void bgtz(int rs, int16_t offset);
        static void blez(int rs, int16_t offset);
        static void bltz(int rs,  int16_t offset);
        static void bne(int rs, int rt, int16_t offset);
        static void lb(int rs, int rt, int16_t offset);
        static void lbu(int rs, int rt, int16_t offset);
        static void lh(int rs, int rt, int16_t offset);
        static void lhu(int rs, int rt, int16_t offset);
        static void lw(int rs, int rt, int16_t offset);
        static void lui(int rt, int16_t imm);
        static void ori(int rs, int rt, int16_t imm);
        static void sb(int rt, int rs, int16_t imm);
        static void slti(int rt, int rs, int16_t imm);
        static void sltiu(int rt, int rs, int16_t imm);
        static void xori(int rt, int rs, int16_t imm);
        static void lwl(int rt, int rs, int16_t imm);
        static void sw(int rt, int rs, int16_t imm);
        static void sh(int rt, int rs, int16_t imm);
        static void lwr(int rt, int rs, int16_t imm);
        static void swl(int rt, int rs, int16_t imm);
        static void swr(int rt, int rs, int16_t imm);
};

class J_operation {
public:
    static void j(int target);
    static void jal(int target);
};



