#ifndef __MyAlgo_H
#define __MyAlgo_H


#include "../AlgorithmBase/AlgorithmBase.h"
class Square;
#include "Square.h"
#include "Parameter.h"
#include <set>
#include <algorithm>
#include <cassert>


class MyAlgo:public AlgorithmBase{
    Square *root;
    vector<Square*> squares;
    Input scaled_input;
    double L, L_plum, ratio;
    double farthest_pair();
public:
    MyAlgo(const Input &_input);
    ~MyAlgo();
    void tree_travse();
    void tree_travse(Square *node);
    void display();
    void rescale(double a, double b);
    void make_tree();
    int get_counter(Square* p);
    map<map<int, int>, double>  get_dp_table();
    double run();
};

#endif