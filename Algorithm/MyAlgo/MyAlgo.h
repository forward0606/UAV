#ifndef __MyAlgo_H
#define __MyAlgo_H


#include"../AlgorithmBase/AlgorithmBase.h"
#include"Parameter.h"
#include<set>
#include<algorithm>
#include<cassert>

struct Portal_id{
    int dir, idx;// dir 邊上的第 idx 個 portal
    Portal_id(int _dir, int _idx);
};


class Square{
    int id;
    vector<Coord> node_list;            // the node in the square
    vector<Coord> portal[4];            // the portal list in the square
    Coord corner[4];
    Square *children[4];
    Square *parent;
    map<map<int, int>, double> dp_table;   // key: 一種路徑選擇，value 為此路徑選擇的最小長度 
    bool dp_table_isable;
    struct Line_Segment_Test{
        static const int ON_LINE = 0;
        static const int LEFT = 1;
        static const int RIGHT = 2;
        static int point_location_test(Coord p1, Coord p2, Coord p3);
        static bool on_segment(Coord a, Coord b, Coord c);
        static bool line_segment_intersection(Coord a, Coord b, Coord c, Coord d);
    };
public:
    inline static int counter;
    inline static vector<Square*> squares;

    /*
        portal 配對用 vector<Portal_id, Portal_id> 存 --> 雙迴圈 for loop
        選 1, 2, ..., 2r 條 path，且每一種組合存成一個 map<int, int> --> {key: portal組合_idx, value: 選幾次}
        把所有組合的 map 塞進一個 vector<map<int, int>> 中，此 vector 為 dp state 中 P 的所有可能
    */
    inline static vector<pair<Portal_id, Portal_id>> portal_pairs;
    inline static vector<map<int, int>> P_sets;
    inline static bool P_sets_isable = false;

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
    void find_portal_pairs();
    void find_P_sets(int path_num, int portal_pair_id, vector<int> &P);
    void find_P_sets();
    map<map<int, int>, double> get_dp_table();
    Square* get_child(int idx);
    int get_id();
    Coord get_Portal_Coord(const Portal_id &p_id)const;
    Square(Coord _upleft, Coord _downright, vector<Coord> _node_list, Square *_parent);
    ~Square();
    bool is_leaf();
    bool is_crossing(vector<int> P);
    bool over_r_limit(vector<int> P);
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
    map<map<int, int>, double>  get_dp_table();
};

#endif