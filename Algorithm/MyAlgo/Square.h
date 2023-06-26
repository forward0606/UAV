#ifndef __Square_H
#define __Square_H


#include "../AlgorithmBase/AlgorithmBase.h"
class MyAlgo;
#include "MyAlgo.h"
#include "Parameter.h"
#include <set>
#include <algorithm>
#include <cassert>
#include <omp.h>
#include <queue>
#define EPS 1e-9
using namespace std;

struct Portal_id{
    int dir, idx;// dir 邊上的第 idx 個 portal
    Portal_id(int _dir, int _idx);
    void display();
};

struct DP_PT{
    map<int, int> p;        // path selection
    int T_id;               // rounding length limit
    bool is_self_cycle;
    DP_PT(map<int, int> _p, int _T_id, bool _is_self_cycle);
};


class Square{
    int id;
    vector<Coord> node_list;            // the node in the square
    vector<Coord> portal[4];            // the portal list in the square
    Coord corner[4];
    Square *children[4];
    Square *parent;
    
    vector<double> T;
    map<int, bool> dp_table;    //key: vector size is k, val is id of DP_PT in all_dp_pts
                                        //val: can we use this parameter to go through all internal node 
    bool dp_table_isable;
    struct Line_Segment_Test{
        static const int ON_LINE = 0;
        static const int LEFT = 1;
        static const int RIGHT = 2;
        static int point_location_test(Coord p1, Coord p2, Coord p3);
        static bool on_segment(Coord a, Coord b, Coord c);
        static bool line_segment_intersection(Coord a, Coord b, Coord c, Coord d);
    };
    MyAlgo *algoptr;    // this square is used by algoptr
public:
    // inline static int counter;
    // inline static vector<Square*> squares;

    /*
        portal 配對用 vector<Portal_id, Portal_id> 存 --> 雙迴圈 for loop
        選 1, 2, ..., 2r 條 path，且每一種組合存成一個 map<int, int> --> {key: portal組合_idx, value: 選幾次}
        把所有組合的 map 塞進一個 vector<map<int, int>> 中，此 vector 為 dp state 中 P 的所有可能
    */
    inline static vector<pair<Portal_id, Portal_id>> portal_pairs;
    inline static vector<map<int, int>> P_sets;
    inline static bool P_sets_isable = false;
    inline static vector<vector<DP_PT>> all_dp_pts;
    inline static bool all_dp_pts_isable = false;
    inline static int z = 0;

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
    void find_all_dp_pts(vector<DP_PT> &states);
    void find_all_dp_pts();
    void set_id(int _id);
    map<int, bool> get_dp_table();
    Square* get_child(int idx);
    int get_id();
    Coord get_Portal_Coord(const Portal_id &p_id)const;
    Square(Coord _upleft, Coord _downright, vector<Coord> _node_list, Square *_parent, MyAlgo * ptr);
    ~Square();
    bool is_leaf();
    bool is_crossing(vector<int> P);
    bool over_r_limit(vector<int> P);
    int self_cycle_check(const DP_PT &s0, const DP_PT &s1, const DP_PT &s2, const DP_PT &s3);
    bool allow_merge(const DP_PT &big, const DP_PT &s0, const DP_PT &s1, const DP_PT &s2, const DP_PT &s3);
};


#endif