#include<iostream>
#include<vector>
using namespace std;
using Coord = pair<double, double>;
#define x first
#define y second

class Input{
    int num_of_node;
    vector<Coord> nodes;
    double B;
public:
    Input(int num, double _B);
    ~Input(){};
    const int& get_num_of_node();
    const vector<Coord>& get_nodes();
    const double& get_B();
    void display();
};