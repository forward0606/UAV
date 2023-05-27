#ifndef _INPUT_H
#define _INPUT_H

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
    Input(const int &n);
    Input(){};
    ~Input(){};
    void set_num_of_node(int n);
    void set_nodes(vector<Coord> v);
    void set_B(double b);
    const int& get_num_of_node()const;
    const vector<Coord>& get_nodes()const;
    const double& get_B()const;
    void display()const;
};

#endif