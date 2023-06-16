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
    /*
        先找 L --> max(mxx-mix, mxy-miy)
        縮放 ceiling(8n/epsilon) / L 倍 
        令 block = 4
        把縮放後的每個點(x, y) -> (round(x/block)*block, round(y/block)*block)
    */
    // find L
    Coord up_left = input.get_nodes()[0], down_right = input.get_nodes()[0];
    for(auto i : input.get_nodes()){
        up_left.x = min(up_left.x, i.x);
        down_right.x = max(down_right.x, i.x);
        up_left.y = max(up_left.y, i.y);
        down_right.y = min(down_right.y, i.y);
    }
    // L = max(x axis distance, y axis distance)
    double L = max(down_right.x - up_left.x, up_left.y - down_right.y);
    // node num
    int n = input.get_nodes().size();
    scaled_input.set_num_of_node(n);
    // set cost B
    scaled_input.set_B(input.get_B());
    // scale L * ratio =  ceil(8n/eplsion) 
    double ratio = ceil(8 * n / par.epsilon) / L;
    // each square size
    double block = 4; 
    // scale coordinate with ratio and shift the min x to y axis, min y to x axis
    // rounding to block
    vector<Coord> v;
    for(auto i : input.get_nodes()){
        // 超越？！
        v.emplace_back(round((i.x-up_left.x) * ratio / block) * block, round((i.y-down_right.y) * ratio / block) * block);
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
    Coord up_left = scaled_input.get_nodes()[0], down_right=scaled_input.get_nodes()[0];
    for(auto i:scaled_input.get_nodes()){
        up_left.x = min(up_left.x, i.x);
        down_right.x = max(down_right.x, i.x);
        up_left.y = max(up_left.y, i.y);
        down_right.y = min(down_right.y, i.y);
    }
    double L = max(down_right.x - up_left.x, up_left.y - down_right.y);
    double L_plum  = 1;
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

Portal_id::Portal_id(int _dir, int _idx)
    :dir(_dir), idx(_idx){}

int Square::get_id(){
    return id;
}


Square *Square::get_child(int idx){
    return children[idx];
}

void Square::find_portal_pairs(){
    portal_pairs.clear();
    vector<Portal_id> v;
    for(int dir=0;dir<4;dir++){
        for(int i=0;i<=par.m;i++){
            v.emplace_back(dir, i);
        }
    }
    for(int idx1 = 0;idx1<(int)v.size();idx1++){
        for(int idx2 = idx1+1;idx2<(int)v.size();idx2++){
            portal_pairs.emplace_back(v[idx1], v[idx2]);
        }
    }
}

int Square::Line_Segment_Test::point_location_test(Coord p1, Coord p2, Coord p3){
    // check the location of p3 is on the (LEFT/RIGHT/TOUCH) of the line
    
    assert(p1 != p2);                                           //no line found
    pair<double, double> u = {p2.x - p1.x, p2.y - p1.y};
    pair<double, double> v = {p3.x - p1.x, p3.y - p1.y};
    double cross_product = u.x * v.y - u.y * v.x;
    if(cross_product == 0){
        return ON_LINE;
    }else if(cross_product > 0){
        return LEFT;
    }else{
        return RIGHT;
    }
}

bool Square::Line_Segment_Test::on_segment(Coord a, Coord b, Coord c){
    // c is on the segment or not
    // a, b, c are located on  the same line
    if(a.x > b.x)   swap(a, b);
    if(a.x <= c.x && c.x <= b.x && min(a.y, b.y) <= c.y && c.y <= max(a.y, b.y)){
        return true;
    }
    return false;
}
bool Square::Line_Segment_Test::line_segment_intersection(Coord a, Coord b, Coord c, Coord d){
    int o1 = point_location_test(a, b, c);
    int o2 = point_location_test(a, b, d);
    int o3 = point_location_test(c, d, a);
    int o4 = point_location_test(c, d, b);
    
    if(o1 != o2 && o3 != o4){return true;}
    if(o1 == 0 && on_segment(a, b, c)){return true;}
    if(o2 == 0 && on_segment(a, b, d)){return true;}
    if(o3 == 0 && on_segment(c, d, a)){return true;}
    if(o4 == 0 && on_segment(c, d, b)){return true;}
    return false;
}

bool Square::is_crossing(vector<int> P){
    pair<Portal_id, Portal_id> last = portal_pairs[P.back()];
    Coord a = get_Portal_Coord(last.first);
    Coord b = get_Portal_Coord(last.second);
    for(int i=0;i<(int)P.size()-1;i++){
        pair<Portal_id, Portal_id> path = portal_pairs[P[i]];
        Coord c = get_Portal_Coord(path.first);
        Coord d = get_Portal_Coord(path.second);
        if(Line_Segment_Test::line_segment_intersection(a, b, c, d)){
           return true; 
        }
    }
    return false;
}

bool Square::over_r_limit(vector<int> P){
    int dir_cnt[4] = {0};
    for(int i=0;i<(int)P.size();i++){
        pair<Portal_id, Portal_id> path = portal_pairs[P[i]];
        dir_cnt[path.first.dir] ++;
        dir_cnt[path.second.dir] ++;
    }
    for(int dir=0;dir<4;dir++){
        if(dir_cnt[dir] > par.r)  return true;
    }
    return false;
}

Coord Square::get_Portal_Coord(const Portal_id &p_id)const{
    return portal[p_id.dir][p_id.idx];
}

void Square::find_P_sets(int path_num, int start, vector<int> &P){
    // check crossing
    if(is_crossing(P)){
        return;
    }
    if(over_r_limit(P)){
        return;
    }
    // store the state
    map<int, int> mp;
    for(int i=0;i<(int)P.size();i++){
        mp[P[i]]++;
    }
    P_sets.emplace_back(mp);

    if(path_num == 2 * par.r){
        return;
    }
    for(int idx = start;idx<(int)portal_pairs.size();idx++){
        P[path_num] = idx;
        find_P_sets(path_num+1, idx, P);
    }
}

void Square::find_P_sets(){
    /*
        先把 portal 做兩兩配對
        portal 配對用 vector<Portal_id, Portal_id> 存 --> 雙迴圈 for loop
        檢查 cross --> 剪枝
        檢查同一條邊走得次數是否超過 r --> 剪枝
        選 1, 2, ..., 2r 條 path，且每一種組合存成一個 map<int, int> --> {key: portal組合_idx, value: 選幾次}
        把所有組合的 map 塞進一個 vector<map<int, int>> 中，此 vector 為 dp state 中 P 的所有可能
	    把 vector<Portal_id, Portal_id>,  vector<map<int, int>> 存成 global 讓所有 dp state 都可以存取
    */
    if(P_sets_isable)return;
    find_portal_pairs();
    vector<int> P(2 * par.r);
    find_P_sets(0, 0, P);
    P_sets_isable = true;
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

map<map<int, int>, double> Square::get_dp_table(){
    cout<<"Square::get_dp_table()"<<endl;
    cout<<"id "<<id<<endl;
    if(dp_table_isable){
        return dp_table;
    }
    dp_table.clear();
    if(node_list.size() == 0){
        cout<<"node_list == 0"<<endl;
        for(auto p:P_sets){
            double distance_sum = 0;
            for(auto it:p){
                pair<Portal_id, Portal_id> last = portal_pairs[it.first];
                Coord a = get_Portal_Coord(last.first);
                Coord b = get_Portal_Coord(last.second);
                cout<<"entry(x, y) = ("<<a.x<<", "<<a.y<<"), exit(x, y) = ("<<b.x<<", "<<b.y<<")\n";
                distance_sum += AlgorithmBase::distance(a, b) * it.second;
            }
            cout<<"distance_sum = "<<distance_sum<<'\n';
            dp_table[p] = distance_sum;
        }
        dp_table_isable = true;
        return dp_table;
    }
    //if(node_list)
    for(int i=0;i<4;i++){
        if(children[i] != nullptr){
            children[i]->get_dp_table();
        }
    }
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
    :id(counter++), node_list(_node_list), parent(_parent), dp_table_isable(false){
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