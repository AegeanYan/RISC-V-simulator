//
// Created by 罗皓天 on 2021/6/28.
//

#include "simulator.h"
#include <string>
#include <sstream>
using namespace std;
void simulator::read() {
    RAM.read();
}
void simulator::IF() {
    registor.fetched_instruct = RAM.mem[pc] + (RAM.mem[pc + 1] << 8) + (RAM.mem[pc + 2] << 16) + (RAM.mem[pc + 3] << 24);
    pc += 4;
}
void simulator::ID() {
    registor.decode();
//    cout << "opt= " << registor.opt << " rd= " << registor.rd << " rs1= " << registor.rs1 <<
//    " rs2= " << registor.rs2 << " imme= " << registor.imme << " branch= " << registor.branch <<endl;
}
int simulator::sext(const int &im,const int &maxw) {//maxw 0-based
    if (im < 0)return im;
    int ans = im;
    if (((im >> maxw) & 1) == 0)return ans;
    else{
        for (int i = maxw + 1; i <= 31; ++i) {
            ans += (1 << i);
        }
    }
    return ans;
}
void simulator::EX(int option) {
    switch (option) {
        case 55:{//lui
            //registor.reg[registor.rd] = registor.imme;
            ex_result.rd = registor.rd;
            ex_result.imme = registor.imme;
            ex_result.pc = pc;
            break;
        }
        case 23:{//auipc
//            registor.reg[registor.rd] = pc + sext(registor.imme , 31);
            ex_result.rd = registor.rd;
            ex_result.imme = pc + sext(registor.imme , 31);
            ex_result.pc = pc;
            break;
        }
        case 111:{//jal
//            registor.reg[registor.rd] = pc + 4;
//            pc += sext(registor.imme , 20);
            if (registor.rd != 0)ex_result.imme = pc;
            else ex_result.imme = 0;
            ex_result.pc = pc + sext(registor.imme , 20) - 4;
            ex_result.rd = registor.rd;
            break;
        }
        case 103:{//jalr
//            int t = pc + 4;
//            pc = (registor.reg[registor.rs1] + sext(registor.imme , 11))&~ 1;
//            registor.reg[registor.rd] = t;
            if(registor.rd != 0)ex_result.imme = pc;
            else ex_result.imme = 0;
            ex_result.pc = (registor.reg[registor.rs1] + sext(registor.imme , 11))&~ 1;
            ex_result.rd = registor.rd;
            break;
        }
        case 99:{
            switch (registor.branch) {
                case 0:{//beq
                    if (registor.reg[registor.rs1] == registor.reg[registor.rs2]) //pc += sext(registor.imme , 12);
                        ex_result.pc = pc + sext(registor.imme , 12) - 4;
                    else ex_result.pc = pc;
                    break;
                }
                case 1:{//bne
                    if (registor.reg[registor.rs1] != registor.reg[registor.rs2]) //pc += sext(registor.imme , 12);
                        ex_result.pc = pc + sext(registor.imme , 12) - 4;
                    else ex_result.pc = pc;
                    break;
                }
                case 4:{//blt
                    if (registor.reg[registor.rs1] < registor.reg[registor.rs2]) //pc += sext(registor.imme , 12);
                        ex_result.pc = pc + sext(registor.imme , 12) - 4;
                    else ex_result.pc = pc;
                    break;
                }
                case 5:{//bge
                    if (registor.reg[registor.rs1] >= registor.reg[registor.rs2]) //pc += sext(registor.imme , 12);
                        ex_result.pc = pc + sext(registor.imme , 12) - 4;
                    else ex_result.pc = pc;
                    break;
                }
                case 6:{//bltu
                    if ((unsigned int)registor.reg[registor.rs1] < (unsigned int)registor.reg[registor.rs2]) //pc += sext(registor.imme , 12);
                        ex_result.pc = pc + sext(registor.imme , 12) - 4;
                    else ex_result.pc = pc;
                    break;
                }
                case 7:{//bgeu
                    if ((unsigned int)registor.reg[registor.rs1] >= (unsigned int)registor.reg[registor.rs2]) //pc += sext(registor.imme , 12);
                        ex_result.pc = pc + sext(registor.imme , 12) - 4;
                    else ex_result.pc = pc;
                    break;
                }
                default: ex_result.pc = pc;
            }
            break;
        }
        case 3:{
            switch (registor.branch) {
                case 0:{//lb
//                    int pos = registor.reg[registor.rs1] + sext(registor.imme , 11);
//                    int ans = 0;
//                    ans = RAM.mem[pos];
//                    registor.reg[registor.rd] = sext(ans , 7);
                    ex_result.rd = registor.rd;
                    ex_result.imme = registor.reg[registor.rs1] + sext(registor.imme , 11);
                    ex_result.branch = 0;
                    
                    break;
                }
                case 1:{//lh
//                    int pos = registor.reg[registor.rs1] + sext(registor.imme , 11);
//                    int ans = 0;
//                    ans = RAM.mem[pos] + (RAM.mem[pos + 1] << 8);
//                    registor.reg[registor.rd] = sext(ans , 15);
                    ex_result.rd = registor.rd;
                    ex_result.imme = registor.reg[registor.rs1] + sext(registor.imme , 11);
                    ex_result.branch = 1;
                    break;
                }
                case 2:{//lw
//                    int pos = registor.reg[registor.rs1] + sext(registor.imme , 11);
//                    int ans = 0;
//                    ans = RAM.mem[pos] + (RAM.mem[pos + 1] << 8) + (RAM.mem[pos + 2] << 16) + (RAM.mem[pos + 3] << 24);
//                    registor.reg[registor.rd] = sext(ans , 31);
                    ex_result.rd = registor.rd;
                    ex_result.imme = registor.reg[registor.rs1] + sext(registor.imme , 11);
                    ex_result.branch = 2;
                    break;
                }
                case 4:{//lbu
//                    int pos = registor.reg[registor.rs1] + sext(registor.imme , 11);
//                    int ans = 0;
//                    ans = RAM.mem[pos];
//                    registor.reg[registor.rd] = ans;
                    ex_result.rd = registor.rd;
                    ex_result.imme = registor.reg[registor.rs1] + sext(registor.imme , 11);
                    ex_result.branch = 4;
                    break;
                }
                case 5:{//lhu
//                    int pos = registor.reg[registor.rs1] + sext(registor.imme , 11);
//                    int ans = 0;
//                    ans = RAM.mem[pos] + (RAM.mem[pos + 1] << 8);
//                    registor.reg[registor.rd] = ans;
                    ex_result.rd = registor.rd;
                    ex_result.imme = registor.reg[registor.rs1] + sext(registor.imme , 11);
                    ex_result.branch = 5;
                    break;
                }
                default:break;
            }
            break;
        }
        case 35:{
            switch (registor.branch) {
                case 0:{//sb
//                    int ans = 0;
//                    ans = registor.reg[registor.rs2] & 255;
//                    int pos = registor.reg[registor.rs1] + sext(registor.imme , 11);
//                    RAM.mem[pos] = ans;
                    ex_result.branch = 0;
                    ex_result.imme = registor.reg[registor.rs1] + sext(registor.imme , 11);
                    ex_result.rd = registor.reg[registor.rs2] & 255;//这里rd被我拿来当ans用了
                    break;
                }
                case 1:{//sh
//                    int pos = registor.reg[registor.rs1] + sext(registor.imme , 11);
//                    int l = registor.reg[registor.rs2] & 255;
//                    int r = (registor.reg[registor.rs2] >> 8) & 255;
//                    RAM.mem[pos] = l;
//                    RAM.mem[pos + 1] = r;
                    ex_result.branch = 1;
                    ex_result.imme = registor.reg[registor.rs1] + sext(registor.imme , 11);
                    ex_result.rd = registor.reg[registor.rs2];
                    break;
                }
                case 2:{//sw
//                    int pos = registor.reg[registor.rs1] + sext(registor.imme , 11);
//                    int a = registor.reg[registor.rs2] & 255;
//                    int b = (registor.reg[registor.rs2] >> 8) & 255;
//                    int c = (registor.reg[registor.rs2] >> 16) & 255;
//                    int d = (registor.reg[registor.rs2] >> 24) & 255;
//                    RAM.mem[pos] = a;
//                    RAM.mem[pos + 1] = b;
//                    RAM.mem[pos + 2] = c;
//                    RAM.mem[pos + 3] = d;
                    ex_result.branch = 2;
                    ex_result.imme = registor.reg[registor.rs1] + sext(registor.imme , 11);
                    ex_result.rd = registor.reg[registor.rs2];
                    break;
                }
                default:break;
            }
            break;
        }
        case 19:{
            ex_result.pc = pc;
            switch (registor.branch) {
                case 0:{//addi
                    //registor.reg[registor.rd] = registor.reg[registor.rs1] + sext(registor.imme , 11);
                    ex_result.branch = 0;
                    ex_result.rd = registor.rd;
                    ex_result.imme = registor.reg[registor.rs1] + sext(registor.imme , 11);
                    break;
                }
                case 2:{//slti
                    ex_result.rd = registor.rd;
                    ex_result.branch = 2;
                    if (registor.reg[registor.rs1] < sext(registor.imme , 11))//registor.reg[registor.rd] = 1;
                        ex_result.imme = 1;
                    else ex_result.imme = 0;
                        //registor.reg[registor.rd] = 0;
                    break;
                }
                case 3:{//sltiu
                    ex_result.rd = registor.rd;
                    ex_result.branch = 3;
                    if (registor.reg[registor.rs1] < (unsigned int)sext(registor.imme , 11))//registor.reg[registor.rd] = 1;
                        ex_result.imme = 1;
                    else ex_result.imme = 0;
                        //registor.reg[registor.rd] = 0;
                    break;
                }
                case 4:{//xori
                    ex_result.branch = 4;
                    ex_result.rd = registor.rd;
                    //registor.reg[registor.rd] = registor.reg[registor.rs1] ^ sext(registor.imme , 11);
                    ex_result.imme = registor.reg[registor.rs1] ^ sext(registor.imme , 11);
                    break;
                }
                case 6:{//ori
                    ex_result.branch = 6;
                    ex_result.rd = registor.rd;
                    //registor.reg[registor.rd] = registor.reg[registor.rs1] | sext(registor.imme , 11);
                    ex_result.imme = registor.reg[registor.rs1] ^ sext(registor.imme , 11);
                    break;
                }
                case 7:{//andi
                    ex_result.branch = 7;
                    ex_result.rd = registor.rd;
                    //registor.reg[registor.rd] = registor.reg[registor.rs1] & sext(registor.imme , 11);
                    ex_result.imme = registor.reg[registor.rs1] & sext(registor.imme , 11);
                    break;//hehe 手贱了
                }
                case 1:{//slli
                    ex_result.branch = 1;
                    ex_result.rd = registor.rd;
                    if ((registor.shamt & 32) == 0){
                        //registor.reg[registor.rd] = registor.reg[registor.rs1] << registor.shamt;
                        ex_result.imme = registor.reg[registor.rs1] << registor.shamt;
                    } else{
                        ex_result.imme = registor.reg[registor.rd];//重写一次
                    }
                    break;
                }
                case 5:{//srli
                    ex_result.branch = 5;
                    ex_result.rd = registor.rd;
                    if (registor.l_or_r == 0){
                        if ((registor.shamt & 32) == 0){
                            //registor.reg[registor.rd] = registor.reg[registor.rs1] >> registor.shamt;
                            ex_result.imme = (unsigned int)registor.reg[registor.rs1] >> registor.shamt;
                        } else ex_result.imme = registor.reg[registor.rd];
                    } else{//srai
                        if ((registor.shamt & 32) == 0){
                            //registor.reg[registor.rd] = sext(registor.reg[registor.rs1] >> registor.shamt , 31 - registor.shamt);
                            ex_result.imme = sext((unsigned int)registor.reg[registor.rs1] >> registor.shamt , 31 - registor.shamt);
                        } else ex_result.imme = registor.reg[registor.rd];
                    }
                    break;
                }
                default:break;
            }
            break;
        }
        case 51:{
            ex_result.pc = pc;
            switch (registor.branch) {
                case 0:{//add and sub
                    ex_result.branch = 0;
                    ex_result.rd = registor.rd;
                    if (registor.l_or_r == 0)//registor.reg[registor.rd] = registor.reg[registor.rs1] + registor.reg[registor.rs2];
                        ex_result.imme = registor.reg[registor.rs1] + registor.reg[registor.rs2];
                    else ex_result.imme = registor.reg[registor.rs1] - registor.reg[registor.rs2];
                        //registor.reg[registor.rd] = registor.reg[registor.rs1] - registor.reg[registor.rs2];
                    break;
                }
                case 1:{//sll
                    ex_result.branch = 1;
                    ex_result.rd = registor.rd;
                    ex_result.imme = registor.reg[registor.rs1] << (registor.reg[registor.rs2] & 31);
                    //registor.reg[registor.rd] = registor.reg[registor.rs1] << (registor.reg[registor.rs2] & 31);
                    break;
                }
                case 2:{//slt
                    ex_result.branch = 2;
                    ex_result.rd = registor.rd;
                    if (registor.reg[registor.rs1] < registor.reg[registor.rs2])//registor.reg[registor.rd] = 1;
                        ex_result.imme = 1;
                    else ex_result.imme = 0;
                        //registor.reg[registor.rd] = 0;
                    break;
                }
                case 3:{//sltu
                    ex_result.branch = 3;
                    ex_result.rd = registor.rd;
                    if ((unsigned int)registor.reg[registor.rs1] < (unsigned int)registor.reg[registor.rs2])ex_result.imme = 1;
                        //registor.reg[registor.rd] = 1;
                    else ex_result.imme = 0;
                        //registor.reg[registor.rd] = 0;
                    break;
                }
                case 4:{//xor
                    ex_result.branch = 4;
                    ex_result.rd = registor.rd;
                    ex_result.imme = registor.reg[registor.rs1] ^ registor.reg[registor.rs2];
                    //registor.reg[registor.rd] = registor.reg[registor.rs1] ^ registor.reg[registor.rs2];
                    break;
                }
                case 5:{//srl and sra
                    ex_result.branch = 5;
                    ex_result.rd = registor.rd;
                    if (registor.l_or_r == 0)//registor.reg[registor.rd] = registor.reg[registor.rs1] >> (registor.reg[registor.rs2] & 31);
                        ex_result.imme = registor.reg[registor.rs1] >> (registor.reg[registor.rs2] & 31);
                    else ex_result.imme = sext(registor.reg[registor.rs1] >> (registor.reg[registor.rs2] & 31), 31 - (registor.reg[registor.rs2] & 31));
                        //registor.reg[registor.rd] = sext(registor.reg[registor.rs1] >> (registor.reg[registor.rs2] & 31), 31 - (registor.reg[registor.rs2] & 31));
                    break;
                }
                case 6:{//or
                    ex_result.branch = 6;
                    ex_result.rd = registor.rd;
                    //registor.reg[registor.rd] = registor.reg[registor.rs1] | registor.reg[registor.rs2];
                    ex_result.imme = registor.reg[registor.rs1] | registor.reg[registor.rs2];
                    break;
                }
                case 7:{
                    ex_result.branch = 7;
                    ex_result.rd = registor.rd;
                    //registor.reg[registor.rd] = registor.reg[registor.rs1] & registor.reg[registor.rs2];
                    ex_result.imme = registor.reg[registor.rs1] & registor.reg[registor.rs2];
                    break;
                }
                default:break;
            }
            break;
        }
        default:break;
    }
}
void simulator::MEM(int option) {
    switch (option) {
        case 55:case 23:case 111:case 103:case 19:case 51:{//lui//auipc//jal//jalr
            mem_data.pc = ex_result.pc;
            mem_data.rd = ex_result.rd;
            mem_data.imme = ex_result.imme;
            mem_data.branch = ex_result.branch;
            break;
        }
        case 99:{//b类
            mem_data.pc = ex_result.pc;
            break;
        }
        case 3:{
            switch (ex_result.branch) {
                case 0:{
                    mem_data.rd = ex_result.rd;
                    mem_data.imme = RAM.mem[ex_result.imme];
                    mem_data.branch = ex_result.branch;
                    break;
                }
                case 1:{
                    mem_data.rd = ex_result.rd;
                    mem_data.imme = RAM.mem[ex_result.imme] + (RAM.mem[ex_result.imme + 1] << 8);
                    mem_data.branch = ex_result.branch;
                    break;
                }
                case 2:{
                    mem_data.rd = ex_result.rd;
                    mem_data.imme = RAM.mem[ex_result.imme] + (RAM.mem[ex_result.imme + 1] << 8) + (RAM.mem[ex_result.imme + 2] << 16) + (RAM.mem[ex_result.imme + 3] << 24);
                    mem_data.branch = ex_result.branch;
                    break;
                }
                case 4:{
                    mem_data.rd = ex_result.rd;
                    mem_data.imme = RAM.mem[ex_result.imme];
                    mem_data.branch = ex_result.branch;
                    break;
                }
                case 5:{
                    mem_data.rd = ex_result.rd;
                    mem_data.imme = RAM.mem[ex_result.imme] + (RAM.mem[ex_result.imme + 1] << 8);
                    mem_data.branch = ex_result.branch;
                    break;
                }
                default:break;
            }
            break;
        }
        case 35:{
            switch (ex_result.branch) {
                case 0:{
                    RAM.mem[ex_result.imme] = ex_result.rd;
                    break;
                }
                case 1:{
                    int l = ex_result.rd & 255;
                    int r = (ex_result.rd >> 8) & 255;
                    RAM.mem[ex_result.imme] = l;
                    RAM.mem[ex_result.imme + 1] = r;
                    break;
                }
                case 2:{
                    int a = ex_result.rd & 255;
                    int b = (ex_result.rd >> 8) & 255;
                    int c = (ex_result.rd >> 16) & 255;
                    int d = (ex_result.rd >> 24) & 255;
                    RAM.mem[ex_result.imme] = a;
                    RAM.mem[ex_result.imme + 1] = b;
                    RAM.mem[ex_result.imme + 2] = c;
                    RAM.mem[ex_result.imme + 3] = d;
                    break;
                }
                default:break;
            }
            break;
        }
    }
}
void simulator::WB(int option) {
    switch (option) {
        case 55:case 23:case 111:case 103:case 19:case 51:{//lui//auipc//jal
            registor.reg[mem_data.rd] = mem_data.imme;
            pc = mem_data.pc;
            break;
        }
        case 99:{
            pc = mem_data.pc;
            break;
        }
        case 3:{
            switch (mem_data.branch) {
                case 0:{
                    registor.reg[mem_data.rd] = sext(mem_data.imme , 7);
                    break;
                }
                case 1:{
                    registor.reg[mem_data.rd] = sext(mem_data.imme , 15);
                    break;
                }
                case 2:{
                    registor.reg[mem_data.rd] = sext(mem_data.imme , 31);
                    break;
                }
                case 4:{
                    registor.reg[mem_data.rd] = mem_data.imme;
                    break;
                }
                case 5:{
                    registor.reg[mem_data.rd] = mem_data.imme;
                    break;
                }
                default:break;
            }
        }
        case 35:{
            break;
        }
    }
}
void simulator::run() {
    int j = 1;
    while (true){
        IF();
        ID();
        if (registor.fetched_instruct == 0x0ff00513){
            cout << (((unsigned int)registor.reg[10]) & 255u);
            break;
        }
        EX(registor.opt);
        MEM(registor.opt);
        WB(registor.opt);
//        cout << j++ << ' ' << "pc = " << pc << ' ' << "loa=" << registor.l_or_r << ' ' << registor.opt << ' ' << registor.branch << ' ' << ' ' << registor.rd << ' ' << registor.rs1 << ' ' << registor.rs2 << endl;
//        for (int i = 0; i < 32; ++i) {
//            cout << "reg[" << i << "]=" << registor.reg[i] << endl;
//        }
    }
}