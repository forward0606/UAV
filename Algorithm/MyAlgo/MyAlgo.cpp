#include"MyAlgo.h"


MyAlgo::MyAlgo(const Input &_input):AlgorithmBase("MyAlgo", _input){}

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
    // set cost B
    scaled_input.set_B(input.get_B());
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
    for(auto e:s){
        v.emplace_back(e);
    }
    scaled_input.set_nodes(v);
}

double MyAlgo::run(){
    double mi = 1e9;
    for(int a = 0;a<1;a++){
        omp_lock_t writelock;
        omp_init_lock(&writelock);
        #pragma omp parallel for
        for(int b = 0;b<6;b++){
            MyAlgo *p = new MyAlgo(input);
            p->rescale(a, b);
            p->make_tree();
            p->tree_travse();
            auto res = p->get_dp_table();
            auto it = res.find(Square::P_sets[0]);
            if( it != res.end()){
                
                omp_set_lock(&writelock);
                mi  = min(mi, it->second);            
                omp_unset_lock(&writelock);

                cerr<<"("<<a<<", "<<b<<") = "<<it->second<<endl;
            }
            delete p;
        }
    }
    return mi;
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

    for(auto i:squares){
        cout << (i->get_id()) << " ";
    }
    cout << endl;
}

void MyAlgo::make_tree(){
    L = L * ratio;
    L_plum = 1;
    while(L_plum < L){
        L_plum *= 2;
    }
    Square::counter = 0;
    Square::squares.clear();
    root = new Square(Coord(0, L_plum), Coord(L_plum, 0), scaled_input.get_nodes(), nullptr);
    root->make_tree_dfs();
    squares = Square::squares;
}

MyAlgo::~MyAlgo(){
    if(root != nullptr) delete root;
}

map<map<int, int>, double>  MyAlgo::get_dp_table(){
    return root->get_dp_table();
}

