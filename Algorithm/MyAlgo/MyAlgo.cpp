#include"MyAlgo.h"


MyAlgo::MyAlgo(const Input &_input)
    :AlgorithmBase("MyAlgo", _input), root(nullptr), L(0), L_plum(0), ratio(1){
}

double MyAlgo::farthest_pair(){
    double d = 0;
    for(auto i : input.get_nodes()){
        for(auto j : input.get_nodes()){
            d = max(d, distance(i, j));
        }
    }
    return d;
}

void MyAlgo::rescale(double a, double b){// with random shift parameter (a, b)
    /*
        先找 L --> max(mxx-mix, mxy-miy)
        縮放 ceiling(8n/epsilon) / L 倍 
        令 block = 4
        把縮放後的每個點(x, y) -> (round(x/block)*block, round(y/block)*block)
    */
    // find L
    cout<<"get_nodes().size() = "<<input.get_nodes().size()<<'\n';
    Coord up_left = input.get_nodes()[0], down_right = input.get_nodes()[0];
    for(auto i : input.get_nodes()){
        up_left.x = min(up_left.x, i.x);
        down_right.x = max(down_right.x, i.x);
        up_left.y = max(up_left.y, i.y);
        down_right.y = min(down_right.y, i.y);
    }
    // L = max(x axis distance, y axis distance)
    L = max(down_right.x - up_left.x, up_left.y - down_right.y);
    // node num
    int n = input.get_nodes().size();
    // scale L * ratio =  ceil(8n/eplsion) 
    ratio = ceil(8 * n / Parameter::epsilon) / L;
    // each square size
    double block = 4; 
    // scale coordinate with ratio and shift the min x to y axis, min y to x axis
    // rounding to block
    set<Coord> s;
    vector<Coord> v;
    for(auto i : input.get_nodes()){
        // 超越？！ --> 0
        s.insert(Coord(round((i.x - up_left.x + a) * ratio / block) * block, round((i.y - down_right.y + b) * ratio / block) * block));
    }
    cout<<"after rescaled("<<a<<", "<<b<<")"<<endl;
    cout<<"ratio = "<<ratio<<endl;
    for(auto e:s){
        v.emplace_back(e);
        cout<<e.x<<" "<<e.y<<endl;
    }
    cout<<"new B is "<<input.get_B() * ratio * (1 + Parameter::epsilon)<<endl;
    scaled_input.set_nodes(v);
    L = L * ratio;
    // set cost B
    scaled_input.set_B(input.get_B() * ratio * (1 + Parameter::epsilon));
}

double MyAlgo::run(){
    bool flag = false;
    //omp_lock_t writelock;
    //omp_init_lock(&writelock);
    // #pragma omp parallel for schedule(dynamic,1)
    rescale(0, 0);
    cerr<<"L = "<<L<<endl;
    // #pragma omp parallel for schedule(dynamic, 1) collapse(2)
    for(int a = 0;a<(int)L;a++){
      for(int b = 0;b<(int)L;b++){
        // double a = 2, b = 0;
            cout<<"("<<a<<", "<<b<<") = "<<endl;
            cerr<<"("<<a<<", "<<b<<") = "<<endl;
            MyAlgo p(input);
            p.rescale(a, b);
            p.make_tree();
            p.tree_travse();
            auto res = p.get_dp_table();
            bool is_able = false;
            for(auto it:res){
                cout<<it.first<<'\n';
                vector<DP_PT> state = Square::all_dp_pts[it.first];
                bool is_good_state = true;
                for(DP_PT st:state){
                    if(!st.is_self_cycle){
                        is_good_state = false;
                        break;
                    }
                }
                if(is_good_state){
                    is_able = true;
                    break;
                }
            }
            if(is_able){
                
                //omp_set_lock(&writelock);
                flag = true;          
                //omp_unset_lock(&writelock);
                cerr << "true" << endl;
                // cerr<<"("<<a<<", "<<b<<") = "<<it->second<<endl;
            }
            cerr<<"dp table on root"<<endl;
            for(auto it:res){
                cerr<<"id: "<<it.first<<", value "<<it.second<<endl;
            }
            cerr<<endl;
       }
    }
    return flag;
}

void MyAlgo::tree_travse(){
    tree_travse(root);
}
void MyAlgo::tree_travse(Square *node){
    if(node == nullptr)return;
    node->display();
    for(int i = 0;i < 4;i++){
        tree_travse(node->get_child(i));
    }
}
void MyAlgo::display(){
    cout << "this is MyAlgo!" << endl;
    
    cout << "the origin input" << endl;
    input.display();
    
    cout << "the scaled input:" << endl;
    scaled_input.display();

}

double MyAlgo::get_L(){
    return L;
}

Input MyAlgo::get_scaled_input(){
    return scaled_input;
}

int MyAlgo::get_counter(Square* p){
    squares.emplace_back(p);
    return squares.size()-1;
}

void MyAlgo::make_tree(){
    L_plum = 1;
    while(L_plum < L){
        L_plum *= 2;
    }
    squares.clear();
    root = new Square(Coord(0, L_plum), Coord(L_plum, 0), scaled_input.get_nodes(), nullptr, this, 0);
    root -> set_id(get_counter(root));
    root -> make_tree_dfs();
}

MyAlgo::~MyAlgo(){
    if(root != nullptr) delete root;
    root = nullptr;
}

map<int, bool>  MyAlgo::get_dp_table(){
    return root->get_dp_table();
}

