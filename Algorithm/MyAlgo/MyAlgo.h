#ifndef __MyAlgo_H
#define __MyAlgo_H

#include"../AlgorithmBase/AlgorithmBase.h"
#include"Parameter.h"
#include<set>
#include<algorithm>




class Dp_value{
    int t;
    vector<vector<Coord>> C;
public:
    Dp_value(const int &_t, const vector<vector<Coord>> &_C);
    Dp_value(){};
    bool operator <(const Dp_value &right)const;
};

struct Portal_id{
    int dir, id;// dir 邊上的第 idx 個 portal
    Portal_id(int _dir, int _id);
    Portal_id();
    bool operator<(const Portal_id &right)const;
};

class Square{
    int id;
    vector<Coord> node_list;     // the node in the square
    vector<Coord> portal[4];     // the portal list in the square
    Coord corner[4];
    Square *children[4];
    Square *parent;
    vector<int> choose[4];
    map<map<pair<Portal_id, Portal_id>, double>, Dp_value> dp_table;
    bool table_enable;
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

    map<map<pair<Portal_id, Portal_id>, double>, Dp_value> find_dp_table();
    //Dp_value find_dp_table(map<pair<Portal_id, Portal_id>, double>);
    void make_tree_dfs();
    void display();
    Square* get_child(int idx);
    int get_id();
    void f(int dir, int id);
    void go_permutation();
    double distance_with_bound(const Portal_id &a, const Portal_id &b)const;
    double myround(double dis)const;
    Coord get_Portal_Coord(const Portal_id &p_id)const;
    Square(Coord _upleft, Coord _downright, vector<Coord> _node_list, Square *_parent);
    ~Square();
    bool is_leaf();
};



class MyAlgo:public AlgorithmBase{
    Square *root;
    vector<Square*> squares;
    Input scaled_input;
    
    double farthest_pair();
public:
    MyAlgo(const Input &_input);
    ~MyAlgo();
    void tree_travse();
    void tree_travse(Square *node);
    void display();
    void Rescale();
    void make_tree();
};

#endif