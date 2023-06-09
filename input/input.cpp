#include "input.h"

const long long MAX_VALUE = 10000;

Input::Input(int num, double _B):B(_B){
    // 產圖 random
    nodes.clear();
    for(int i = 0; i < num; i++){
        double a = rand() % MAX_VALUE;
        a /= rand() % 100;
        double b = rand() % MAX_VALUE;
        b /= rand() % 100;
        nodes.emplace_back(a, b);
    }
}
Input::Input(const Input& r){
    // #我下次會乖乖寫這個
    nodes.clear();
    B = r.B;
    for(auto e:r.nodes){
        nodes.emplace_back(e);
    }
}


Input::Input(){
    B = 1.5;
    nodes.clear();
    nodes.emplace_back(12, 12);
    nodes.emplace_back(18, 12);
    nodes.emplace_back(32, 32);
    //nodes.emplace_back(0, 64);
    //nodes.emplace_back(64, 64);
    //nodes.emplace_back(64, 0);
    // nodes.emplace_back(10, 10);
    // nodes.emplace_back(16, 2);
    // nodes.emplace_back(27, 23);
    // nodes.emplace_back(79, 23);
    // nodes.emplace_back(87, 61);
    // nodes.emplace_back(64, 64);
}

int Input::get_num_of_node()const{
    return nodes.size();
}
const vector<Coord>& Input::get_nodes()const{
    return nodes;
}
const double& Input::get_B()const{
    return B;
}

void Input::set_nodes(vector<Coord> v){
    nodes = v;
}
void Input::set_B(double b){
    B = b;
}

void Input::display()const{
    cout<<"num_of_node, B"<<endl;
    cout<<nodes.size()<<", "<<B<<endl;
    cout<<"list of node: x, y"<<endl;
    for(auto u:nodes){
        cout<<u.x<<" "<<u.y<<endl;
    }
}
