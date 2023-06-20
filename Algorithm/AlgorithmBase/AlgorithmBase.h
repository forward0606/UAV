#ifndef __ALGORITHMBASE_H
#define __ALGORITHMBASE_H

#include<iostream>
#include<map>
#include"../../input/input.h"
#include<cmath>
using namespace std;


class AlgorithmBase{
    string algorithm_name;
protected:
    Input input;
    map<string, double> result;
public:
    AlgorithmBase(const string &algo_name, const Input &_input);
    // ~AlgorithmBase(){};
    string get_algo_name();
    static double distance(Coord p1, Coord p2);
    // void run();
};

#endif