//
// Created by 罗皓天 on 2021/6/28.
//

#ifndef RISC_V_SIMULATOR_SIMULATOR_H
#define RISC_V_SIMULATOR_SIMULATOR_H

#include <iostream>
#include "Memory.h"
#include "ALU.h"
#include "Registor.h"
class simulator {
private:
    Registor registor;
    ALU alu;
    Memory RAM;
    int pc;
    struct EX_Result{
    public:
        int rd;
        int imme;
        int branch;
        int pc;
        EX_Result() = default;
    };
    struct MEM_Data{
    public:
        int rd;
        int imme;
        int branch;
        int pc;
        MEM_Data() = default;
    };
    EX_Result ex_result{};
    MEM_Data mem_data{};
public:
    simulator() {
        pc = 0;
    }
    void read();
    void IF();
    void ID();
    void EX(int option);
    void MEM(int option);
    void WB(int option);
    void run();
    static int sext(const int &im ,const int &maxw);
};


#endif //RISC_V_SIMULATOR_SIMULATOR_H
