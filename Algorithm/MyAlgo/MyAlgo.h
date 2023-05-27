#ifndef __MyAlgo_H
#define __MyAlgo_H

#include"../AlgorithmBase/AlgorithmBase.h"
#include"Parameter.h"
#include<set>
#include<algorithm>


class Square{
    int id;
    vector<Coord> node_list;     // the node in the square
    vector<Coord> portal[4];     // the portal list in the square
    Coord corner[4];
    Square *children[4];
    Square *parent;
public:
    inline static int counter;
    inline static vector<Square*> squares;

    static const int Idx_U = 0;
    static const int Idx_L = 1;
    static const int Idx_R = 2;
    static const int Idx_D = 3;
    static const int Idx_LU = 0;
    static const int Idx_LD = 1;
    static const int Idx_RU = 2;
    static const int Idx_RD = 3;

    void make_tree_dfs();
    void display();
    Square* get_child(int idx);
    int get_id();
    Square(Coord _upleft, Coord _downright, vector<Coord> _node_list, Square *_parent);
    ~Square();
    bool is_leaf();
};

struct Portal_id{
    int dir, idx;// dir 邊上的第 idx 個 portal
};

class Dp_state{
    int S_id;
    map<pair<Portal_id, Portal_id>, double> PT;
};

class MyAlgo:public AlgorithmBase{
    Square *root;
    vector<Square*> squares;
    Input scaled_input;
    
    double farthest_pair();
public:
    MyAlgo(const Input &_input);
    void tree_travse();
    void tree_travse(Square *node);
    void display();
    void Rescale();
    void make_tree();
};

#endif