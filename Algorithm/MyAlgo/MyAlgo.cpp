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
    // find the x, y position of the biggest square
    for(auto i:scaled_input.get_nodes()){
        upleft.x = min(upleft.x, i.x);
        down_right.x = max(down_right.x, i.x);
        upleft.y = max(upleft.y, i.y);
        down_right.y = min(down_right.y, i.y);
    }
    
    Square::counter = 0;                            // computes the size of the tree
    Square::squares.clear();                        // store every square node in a vector
    root = new Square(upleft, down_right, scaled_input.get_nodes(), nullptr);
    root->make_tree_dfs();
    squares = Square::squares
}

 MyAlgo::~MyAlgo(){
    if(root != nullptr) delete root;
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
    :id(counter++), node_list(_node_list), parent(_parent), table_enable(false){
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

void Square::go_permutation(){
    
    //store all choosed portal in vector v
    vector<Portal_id> v;
    for(int dir = 0; dir < 4; dir++){
        for(int id = 0; id < par.r; id++){
            if(choose[dir][id] != -1){
                v.emplace_back(dir, id);
            }
        }
    }
    
    // construct PT table
    map<pair<Portal_id, Portal_id>, double> mp;
    sort(v.begin(), v.end());                   // permutation from small to big
    do {
        mp.clear();
        vector<vector<Coord>> C;
        vector<Coord> path;
        for(int i=0;i<(int)v.size();i+=2){
            path.clear();
            if(v[i] < v[i+1])
                mp[make_pair(v[i], v[i+1])] = distance_with_bound(v[i], v[i+1]);
            else
                mp[make_pair(v[i+1], v[i])] = distance_with_bound(v[i], v[i+1]);
            path.emplace_back(get_Portal_Coord(v[i]));
            path.emplace_back(get_Portal_Coord(v[i+1]));
            C.emplace_back(path);
        }
        dp_table[mp] = Dp_value(0, C);
    } while (next_permutation(v.begin(), v.end()));
}

double Square::distance_with_bound(const Portal_id &a, const Portal_id &b)const{
    // return rounded distance between two portals
    Coord p1 = get_Portal_Coord(a);
    Coord p2 = get_Portal_Coord(b);
    double dis = AlgorithmBase::distance(p1, p2);
    return myround(dis);
}

double Square::myround(double dis)const{
    // what's round function?
    return dis;
}


Coord Square::get_Portal_Coord(const Portal_id &p_id)const{
    return portal[p_id.dir][p_id.id];
}

void Square::f(int dir, int id){
    // recursize for choose porta
    if(id == par.r){
        dir += 1;
        id = 0;
    }
    if(dir == 4){
        //find a case
        go_permutation();
        return;
    }
    f(dir, id+1);               //not choose
    for(int i=0;i<=par.m;i++){
        choose[dir][id] = i;    //choose ith portal
        f(dir, id+1);
    }
}

map<map<pair<Portal_id, Portal_id>, double>, Dp_value> Square::find_dp_table(){
    if(table_enable){
        // the dp_table is construct before
        return dp_table;
    }
    
    if(is_leaf()){
        for(int dir=0;dir<4;dir++){
            choose[dir].resize(par.r, -1);
        }
        //choose[dir][j] --> dir 的邊的第 j 個選擇的 id

    }
    return dp_table;
}


Dp_value::Dp_value(const int &_t, const vector<vector<Coord>> &_C)
        :t(_t), C(_C){}
bool Dp_value::operator <(const Dp_value &right)const{
    if(t != right.t){
        return t < right.t;
    }
    return C.size() < right.C.size();
}

Portal_id::Portal_id(int _dir, int _id)
    :dir(_dir), id(_id){}
Portal_id::Portal_id(){}
bool Portal_id::operator<(const Portal_id &right)const{
    if(dir != right.dir)    return dir < right.dir;
    return id < right.id;
}