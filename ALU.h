//
// Created by 罗皓天 on 2021/6/29.
//

#ifndef RISC_V_SIMULATOR_ALU_H
#define RISC_V_SIMULATOR_ALU_H


class ALU {
    bool overflow;
    bool zero;
    bool negative;
public:
    ALU():overflow(false),zero(false),negative(false){};
};


#endif //RISC_V_SIMULATOR_ALU_H
