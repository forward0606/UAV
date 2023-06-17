#include "Square.h"



Portal_id::Portal_id(int _dir, int _idx)
    :dir(_dir), idx(_idx){}

void Portal_id::display(){
    cout << "This is a portal: dir = " << dir << ", idx = " << idx << endl;
}

int Square::get_id(){
    return id;
}

Square *Square::get_child(int idx){
    return children[idx];
}

void Square::find_portal_pairs(){
    cout << "find portal pairs" << endl;
    portal_pairs.clear();
    vector<Portal_id> v;
    for(int dir = 0;dir < 4;dir++){
        for(int i = 0;i <= Parameter::m;i++){
            v.emplace_back(dir, i);
        }
    }
    for(int idx1 = 0;idx1 < (int)v.size();idx1++){
        for(int idx2 = idx1;idx2 < (int)v.size();idx2++){
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
    if(a.x > b.x){
        swap(a, b);
    }
    if(a.x <= c.x && c.x <= b.x && min(a.y, b.y) <= c.y && c.y <= max(a.y, b.y)){
        return true;
    }
    return false;
}
bool Square::Line_Segment_Test::line_segment_intersection(Coord a, Coord b, Coord c, Coord d){
    if(a == b && c == d){
        return a == c;
    }
    if(a == b){
        //(c, d) is a line, a is a point
        return point_location_test(c, d, a) == ON_LINE && on_segment(c, d, a);
    }
    if(c == d){
        return point_location_test(a, b, c) == ON_LINE && on_segment(a, b, c);
    }
    
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
    if(P.size() <= 1){
        return false;
    }
    pair<Portal_id, Portal_id> last = portal_pairs[P.back()];
    Coord a = get_Portal_Coord(last.first);
    Coord b = get_Portal_Coord(last.second);
    for(int i = 0;i < (int)P.size() - 1;i++){
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
    for(int i = 0;i < (int)P.size();i++){
        pair<Portal_id, Portal_id> path = portal_pairs[P[i]];
        dir_cnt[path.first.dir]++;
        dir_cnt[path.second.dir]++;
    }
    for(int dir = 0;dir < 4;dir++){
        if(dir_cnt[dir] > Parameter::r){
            return true;
        }
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
    for(int i = 0;i < (int)P.size();i++){
        mp[P[i]]++;
    }
    P_sets.emplace_back(mp);

    if(path_num == 2 * Parameter::r){
        return;
    }
    for(int idx = start;idx < (int)portal_pairs.size();idx++){
        P.emplace_back(idx);
        find_P_sets(path_num + 1, idx, P);
        P.pop_back();
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
    if(P_sets_isable){
        return;
    }
    find_portal_pairs();
    vector<int> P;
    find_P_sets(0, 0, P);
    P_sets_isable = true;
}

void Square::display(){
    cout << "this is a square!" << endl;
    cout << "id: " << id << endl;
    cout << "node list size " << node_list.size() << endl;
    cout << "node list:" << endl;
    for(auto i : node_list){
        cout << i.x << " " << i.y << endl;
    }
    cout << "portal:" << endl;
    for(int j = 0;j < 4;j++){
        if(Idx_L == j){
            cout << "L:" << endl;
        }else if(Idx_D == j){
            cout << "D:" << endl;
        }else if(Idx_U == j){
            cout << "U:" << endl;
        }else{
            cout << "R:" << endl;
        }
        for(auto i : portal[j]){
            cout << i.x << " " << i.y << endl;
        }
        cout << endl;
    }
    cout << "corner:" << endl;
    for(int j = 0;j < 4;j++){
        if(Idx_LD == j){
            cout << "LD: ";
        }else if(Idx_RD == j){
            cout << "RD: ";
        }else if(Idx_LU == j){
            cout << "LU: ";
        }else{
            cout << "RU: ";
        }
        cout << corner[j].x << " " << corner[j].y << endl;
    }
    cout << "child id" << endl;
    for(int j = 0;j < 4;j++){
        if(Idx_LD == j){
            cout << "LD: ";
        }else if(Idx_RD == j){
            cout << "RD: ";
        }else if(Idx_LU == j){
            cout << "LU: ";
        }else{
            cout << "RU: ";
        }
        cout << (children[j] == nullptr ? -1 : children[j]->id) << endl;
    }
    cout << "parent id: " << (parent == nullptr ? -1 : parent->id) << endl;
}

bool Square::allow_merge(const map<int, int> &big, const map<int, int> &s0,\
                         const map<int, int> &s1, const map<int, int> &s2, const map<int, int> &s3){
    map<Coord, int> cnt_node_degree;

    for(auto i:big){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = get_Portal_Coord(path.first);
        Coord b = get_Portal_Coord(path.second);
        cnt_node_degree[a]++;
        cnt_node_degree[b]++;
    }

    for(auto i:s0){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = children[0]->get_Portal_Coord(path.first);
        Coord b = children[0]->get_Portal_Coord(path.second);
        cnt_node_degree[a]++;
        cnt_node_degree[b]++;
    }

    for(auto i:s1){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = children[1]->get_Portal_Coord(path.first);
        Coord b = children[1]->get_Portal_Coord(path.second);
        cnt_node_degree[a]++;
        cnt_node_degree[b]++;
    }

    for(auto i:s2){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = children[2]->get_Portal_Coord(path.first);
        Coord b = children[2]->get_Portal_Coord(path.second);
        cnt_node_degree[a]++;
        cnt_node_degree[b]++;
    }

    for(auto i:s3){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = children[3]->get_Portal_Coord(path.first);
        Coord b = children[3]->get_Portal_Coord(path.second);
        cnt_node_degree[a]++;
        cnt_node_degree[b]++;
    }

    for(auto i:cnt_node_degree){
        if(i.second & 1){
            return false;
        }
    }
    return true;
}


map<map<int, int>, double> Square::get_dp_table(){
    cout << "Square::get_dp_table()" << endl;
    cout << "id " << id << endl;
    if(dp_table_isable){
        return dp_table;
    }
    dp_table.clear();
    if(node_list.size() == 0){
        cout << "node_list == 0" << endl;
        find_P_sets();
        // for(int i = 0;i < (int)portal_pairs.size();i++){
        //     portal_pairs[i].first.display();
        //     portal_pairs[i].second.display();
        // }
        for(auto p : P_sets){
            double distance_sum = 0;
            for(auto it : p){
                pair<Portal_id, Portal_id> last = portal_pairs[it.first];
                Coord a = get_Portal_Coord(last.first);
                Coord b = get_Portal_Coord(last.second);
                for(int iter = 0; iter < it.second; iter++){
                    cout << "entry(x, y) = (" << a.x << ", " << a.y << "), exit(x, y) = (" << b.x << ", " << b.y << ")" << endl;
                }
                distance_sum += AlgorithmBase::distance(a, b) * it.second;
            }
            cout << "distance_sum = " << distance_sum << endl;
            dp_table[p] = distance_sum;
        }
        dp_table_isable = true;
        return dp_table;
    }
    if(node_list.size() == 1){
        cout << "node_list == 1" << endl;
        cout << "node(x, y) = (" << node_list[0].x << ", " << node_list[0].y << ")" << endl;
        find_P_sets();
        for(auto p : P_sets){
            double distance_sum = 0;
            if(p.begin() == p.end()){
                continue;
            }
            for(auto it : p){
                pair<Portal_id, Portal_id> last = portal_pairs[it.first];
                Coord a = get_Portal_Coord(last.first);
                Coord b = get_Portal_Coord(last.second);
                for(int iter = 0; iter < it.second; iter++){
                    cout << "entry(x, y) = (" << a.x << ", " << a.y << "), exit(x, y) = (" << b.x << ", " << b.y << ")" << endl;
                }
                distance_sum += AlgorithmBase::distance(a, b) * it.second;
            }
            cout << "distance_sum = " << distance_sum << endl;
            double mi = 10 * distance_sum;
            for(auto it:p){
                pair<Portal_id, Portal_id> last = portal_pairs[it.first];
                Coord a = get_Portal_Coord(last.first);
                Coord b = get_Portal_Coord(last.second);
                double pass_node = AlgorithmBase::distance(a, node_list[0]) + AlgorithmBase::distance(b, node_list[0]);
                mi = min(mi, distance_sum - AlgorithmBase::distance(a, b) + pass_node);
            }
            cout << "minimum distance = " << mi << endl;
            dp_table[p] = mi;
        }

        dp_table_isable = true;
        return dp_table;
    }

    // recursive
    vector<map<map<int, int>, double>> children_dp_table(4);
    #pragma omp parallel for
    for(int i = 0;i < 4;i++){
        if(children[i] != nullptr){
            children_dp_table[i] = children[i]->get_dp_table();
        }
    }
    cout << "merge" << endl;
    // merge
    find_P_sets();
    // vector<map<int, int>> P_sets; value: 一組 path 集合
    // map<map<int, int>, double> dp_table key: 一組 path 集合, value: path 集合的總長度
    #pragma omp parallel for
    for(auto p : P_sets){
        double min_distance = 1e9;
        omp_lock_t writelock;
        omp_init_lock(&writelock);
        #pragma omp parallel for
        for(auto p0:children_dp_table[0]){
            #pragma omp parallel for
            for(auto p1: children_dp_table[1]){
                #pragma omp parallel for
                for(auto p2:children_dp_table[2]){
                    #pragma omp parallel for
                    for(auto p3:children_dp_table[3]){
                        if(allow_merge(p, p0.first, p1.first, p2.first, p3.first)){
                            double distance_sum = p0.second + p1.second + p2.second + p3.second;
                            omp_set_lock(&writelock);
                            min_distance = min(distance_sum, min_distance);
                            omp_unset_lock(&writelock);
                        }
                    }
                }
            }
        }
        for(auto it : p){
            pair<Portal_id, Portal_id> last = portal_pairs[it.first];
            Coord a = get_Portal_Coord(last.first);
            Coord b = get_Portal_Coord(last.second);
            cout << "entry(x, y) = (" << a.x << ", " << a.y << "), exit(x, y) = (" << b.x << ", " << b.y << ")" << endl;
        }
        cout<<"min_distance = " << min_distance << endl;
        if(min_distance - 1e9 < EPS){
            dp_table[p] = min_distance;
        }
    }
    dp_table_isable = true;

    return dp_table;
}


void Square::make_tree_dfs(){
    if(is_leaf()){
        return;
    }
    vector<Coord> v[4];
    Coord mid((corner[Idx_LU].x+corner[Idx_RU].x)/2, (corner[Idx_LU].y+corner[Idx_LD].y)/2);

    for(auto i : node_list){
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
    for(int i = 0;i < 4;i++){
        children[i]->make_tree_dfs();
    }
}


Square::Square(Coord _upleft, Coord _downright, vector<Coord> _node_list, Square *_parent)
    :id(counter++), node_list(_node_list), parent(_parent), dp_table_isable(false){
    for(int i = 0;i < 4;i++){
        children[i] = nullptr;
    }
    corner[Idx_LU] = _upleft;
    corner[Idx_LD] = make_pair(_upleft.x, _downright.y);
    corner[Idx_RU] = make_pair(_downright.x, _upleft.y);;
    corner[Idx_RD] = _downright;

    double W = _downright.x - _upleft.x;
    double H = _upleft.y - _downright.y;
    double dx = W / (Parameter::m + 1), dy = H / (Parameter::m + 1);
    for(int i = 0;i <= Parameter::m;i++){
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
    for(int i = 0;i < 4;i++){
        if(children[i] != nullptr){
            delete children[i];
            children[i] = nullptr;
        }
    }
}