//
// Created by 罗皓天 on 2021/6/28.
//
#include <iostream>
#include "simulator.h"
#include <string>
#include <cstdio>
using namespace std;
int main(){
    freopen("in" , "r" , stdin);
    freopen("out" , "w" , stdout);
    simulator s;
    s.read();
    //cout << 12222222;
    s.run();
}