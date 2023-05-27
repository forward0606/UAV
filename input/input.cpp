#include "input.h"

const long long MAX_VALUE = 1e9;

Input::Input(int num, double _B):num_of_node(num), B(_B){
    // 產圖 random
    for(int i = 0; i < num_of_node; i++){
        double a = rand() % MAX_VALUE;
        a /= rand() % MAX_VALUE;
        double b = rand() % MAX_VALUE;
        b /= rand() % MAX_VALUE;
        nodes.emplace_back(a, b);
    }
}
const int& Input::get_num_of_node(){
    return num_of_node;
}
const vector<Coord>& Input::get_nodes(){
    return nodes;
}
const double& Input::get_B(){
    return B;
}

void Input::display(){
    cout<<"num_of_node, B"<<endl;
    cout<<num_of_node<<", "<<B<<endl;
    cout<<"list of node: x, y"<<endl;
    for(auto u:nodes){
        cout<<u.x<<", "<<u.y<<endl;
    }
}
