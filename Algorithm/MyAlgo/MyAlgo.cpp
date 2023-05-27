#include"MyAlgo.h"

Parameter par;

MyAlgo::MyAlgo(const Input &_input):AlgorithmBase("MyAlgo", _input){}

double MyAlgo::farthest_pair(){
    double d = 0;
    for(auto i: input.get_nodes()){
        for(auto j: input.get_nodes()){
            d = max(d, distance(i, j));
        }
    }
    return d;
}

void MyAlgo::Rescale(){
    //parameter for cutting square length in rounding
    double granularity_grid = par.epsilon * farthest_pair() / (8 * par.k * input.get_num_of_node());
    granularity_grid = 16;
    cout<<"granularity_grid: "<<granularity_grid<<endl;
    scaled_input.set_num_of_node(input.get_num_of_node());
    scaled_input.set_B(input.get_B());
    vector<Coord> v;
    for(auto i: input.get_nodes()){
        //v.emplace_back(round(i.x / granularity_grid) * 8, round(i.y / granularity_grid) * 8);
        v.emplace_back(round(i.x / granularity_grid)*granularity_grid, round(i.y / granularity_grid)*granularity_grid);
    }
    scaled_input.set_nodes(v);
}

void MyAlgo::tree_travse(){
    tree_travse(root);
}
void MyAlgo::tree_travse(Square *node){
    if(node == nullptr)return;
    node->display();
    for(int i=0;i<4;i++){
        tree_travse(node->get_child(i));
    }
}
void MyAlgo::display(){
    cout<<"this is MyAlgo!"<<endl;
    
    cout<<"the origin input"<<endl;
    input.display();
    
    cout<<"the scaled input:"<<endl;
    scaled_input.display();

    for(auto i:squares){
        cout<<(i->get_id())<<" ";
    }
    cout<<endl;
}

void MyAlgo::make_tree(){
    Coord upleft = scaled_input.get_nodes()[0], down_right=scaled_input.get_nodes()[0];
    for(auto i:scaled_input.get_nodes()){
        upleft.x = min(upleft.x, i.x);
        down_right.x = max(down_right.x, i.x);
        upleft.y = max(upleft.y, i.y);
        down_right.y = min(down_right.y, i.y);
    }
    Square::counter = 0;
    Square::squares.clear();
    root = new Square(upleft, down_right, scaled_input.get_nodes(), nullptr);
    root->make_tree_dfs();
    squares = Square::squares;
}

int Square::get_id(){
    return id;
}

Square *Square::get_child(int idx){
    return children[idx];
}

void Square::display(){
    cout<<"this is a square!"<<endl;
    cout<<"id: "<<id<<endl;
    cout<<"node list size "<<node_list.size()<<endl;
    cout<<"node list:"<<endl;
    for(auto i:node_list){
        cout<<i.x<<" "<<i.y<<endl;
    }
    cout<<"portal:"<<endl;
    for(int j=0;j<4;j++){
        if(Idx_L == j){
            cout<<"L:"<<endl;
        }else if(Idx_D == j){
            cout<<"D:"<<endl;
        }else if(Idx_U == j){
            cout<<"U:"<<endl;
        }else{
            cout<<"R:"<<endl;
        }
        for(auto i:portal[j]){
            cout<<i.x<<" "<<i.y<<endl;
        }
        cout<<endl;
    }
    cout<<"corner:"<<endl;
    for(int j=0;j<4;j++){
        if(Idx_LD == j){
            cout<<"LD: ";
        }else if(Idx_RD == j){
            cout<<"RD: ";
        }else if(Idx_LU == j){
            cout<<"LU: ";
        }else{
            cout<<"RU: ";
        }
        cout<<corner[j].x<<" "<<corner[j].y<<endl;
    }
    cout<<"child id"<<endl;
    for(int j=0;j<4;j++){
        if(Idx_LD == j){
            cout<<"LD: ";
        }else if(Idx_RD == j){
            cout<<"RD: ";
        }else if(Idx_LU == j){
            cout<<"LU: ";
        }else{
            cout<<"RU: ";
        }
        cout<<(children[j]==nullptr?-1:children[j]->id)<<endl;
    }
    cout<<"parent id: "<<(parent==nullptr?-1:parent->id)<<endl;
}

void Square::make_tree_dfs(){
    if(is_leaf()){
        return;
    }
    vector<Coord> v[4];
    Coord mid((corner[Idx_LU].x+corner[Idx_RU].x)/2, (corner[Idx_LU].y+corner[Idx_LD].y)/2);

    for(auto i:node_list){
        if(i.x <= mid.x && i.y >= mid.y){
            v[Idx_LU].emplace_back(i);
        }
        else if(i.x <= mid.x){
            v[Idx_LD].emplace_back(i);
        }else if(i.y >= mid.y){
            v[Idx_RU].emplace_back(i);
        }else{
            v[Idx_RD].emplace_back(i);
        }
    }
    children[Idx_LU] = new Square(corner[Idx_LU], mid, v[Idx_LU], this);
    children[Idx_LD] = new Square(make_pair(corner[Idx_LD].x, mid.y), make_pair(mid.x, corner[Idx_LD].y), v[Idx_LD], this);
    children[Idx_RU] = new Square(make_pair(mid.x, corner[Idx_RU].y), make_pair(corner[Idx_RU].x, mid.y), v[Idx_RU], this);
    children[Idx_RD] = new Square(mid, corner[Idx_RD], v[Idx_RD], this);
    for(int i=0;i<4;i++){
        children[i]->make_tree_dfs();
    }
}


Square::Square(Coord _upleft, Coord _downright, vector<Coord> _node_list, Square *_parent)
    :id(counter++), node_list(_node_list), parent(_parent){
    for(int i=0;i<4;i++){
        children[i] = nullptr;
    }
    corner[Idx_LU] = _upleft;
    corner[Idx_LD] = make_pair(_upleft.x, _downright.y);
    corner[Idx_RU] = make_pair(_downright.x, _upleft.y);;
    corner[Idx_RD] = _downright;

    double W = _downright.x - _upleft.x;
    double H = _upleft.y - _downright.y;
    double dx = W / (par.m + 1), dy = H / (par.m + 1);
    for(int i=0;i<=par.m;i++){
        portal[Idx_L].emplace_back(corner[Idx_LD].x, corner[Idx_LD].y + dy * i);
        portal[Idx_U].emplace_back(corner[Idx_LU].x + dx * i, corner[Idx_LU].y);
        portal[Idx_R].emplace_back(corner[Idx_RU].x, corner[Idx_RU].y - dy * i);
        portal[Idx_D].emplace_back(corner[Idx_RD].x - dx * i, corner[Idx_RD].y);
    }
    squares.emplace_back(this);
}


bool Square::is_leaf(){
    return node_list.size() <= 1;
}

Square::~Square(){
    for(int i=0;i<4;i++){
        if(children[i] != nullptr){
            delete children[i];
            children[i] = nullptr;
        }
    }
}