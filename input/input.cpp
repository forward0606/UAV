#include "input.h"

const long long MAX_VALUE = 10000;

Input::Input(int num, double _B):num_of_node(num), B(_B){
    // 產圖 random
    for(int i = 0; i < num_of_node; i++){
        double a = rand() % MAX_VALUE;
        a /= rand() % 100;
        double b = rand() % MAX_VALUE;
        b /= rand() % 100;
        nodes.emplace_back(a, b);
    }
}


Input::Input(const int &n){
    num_of_node = n;
    B = 20;
    nodes.emplace_back(1, 1);
    nodes.emplace_back(8, 8);
    nodes.emplace_back(0, 64);
    nodes.emplace_back(64, 64);
    nodes.emplace_back(64, 0);
    // nodes.emplace_back(10, 10);
    // nodes.emplace_back(16, 2);
    // nodes.emplace_back(27, 23);
    // nodes.emplace_back(79, 23);
    // nodes.emplace_back(87, 61);
    // nodes.emplace_back(64, 64);
    num_of_node = nodes.size();
}

const int& Input::get_num_of_node()const{
    return num_of_node;
}
const vector<Coord>& Input::get_nodes()const{
    return nodes;
}
const double& Input::get_B()const{
    return B;
}


void Input::set_num_of_node(int n){
    num_of_node = n;
}
void Input::set_nodes(vector<Coord> v){
    nodes = v;
}
void Input::set_B(double b){
    B = b;
}

void Input::display()const{
    cout<<"num_of_node, B"<<endl;
    cout<<num_of_node<<", "<<B<<endl;
    cout<<"list of node: x, y"<<endl;
    for(auto u:nodes){
        cout<<u.x<<", "<<u.y<<endl;
    }
}
