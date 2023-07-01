#include "Square.h"



Portal_id::Portal_id(int _dir, int _idx)
    :dir(_dir), idx(_idx){}

void Portal_id::display(){
    cout << "This is a portal: dir = " << dir << ", idx = " << idx << endl;
}

DP_PT::DP_PT(int _id, map<int, int> _p, int _T_id, bool _is_self_cycle)
    :id(_id), p(_p), T_id(_T_id), is_self_cycle(_is_self_cycle){}

bool DP_PT::operator<(const DP_PT &r)const{
    if(T_id != r.T_id){
        return T_id < r.T_id;
    }
    return id < r.id;
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
    
    portal_pairs.emplace_back(Portal_id(0, 0), Portal_id(3, 0));
    portal_pairs.emplace_back(Portal_id(1, 0), Portal_id(2, 0));
    return;
    
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

bool Square::over_r_limit(vector<DP_PT> states){
    int dir_cnt[4] = {0};
    for(DP_PT st:states){
        for(pair<int, int> path_id:st.p){
            pair<Portal_id, Portal_id> path = portal_pairs[path_id.first];
            dir_cnt[path.first.dir]++;
            dir_cnt[path.second.dir]++;
        }
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

void Square::find_all_dp_pts(vector<DP_PT> &states){
    
    if(over_r_limit(states)){
        return;
    }

    if(states.size() == Parameter::k){
        all_dp_pts.emplace_back(states);
        return;
    }

    for(int t_id=0;t_id<z;t_id++){
        map<int, int> p;
        if(states.size() >= 1 && DP_PT(-1, p, t_id, true) < states.back()){
            continue;
        }
        states.emplace_back(-1, p, t_id, true);
        find_all_dp_pts(states);
        states.pop_back();
    }
    
    for(int p_id = 0;p_id < (int)P_sets.size();p_id++){
        map<int, int> p  = P_sets[p_id];
        for(int t_id=0;t_id<z;t_id++){
            if(states.size() >= 1 && DP_PT(p_id, p, t_id, false) < states.back()){
                continue;
            }
            states.emplace_back(p_id, p, t_id, false);
            find_all_dp_pts(states);
            states.pop_back();
        }
    }
}

void Square::find_all_dp_pts(){
    find_P_sets();
    if(all_dp_pts_isable){
        return;
    }
    vector<DP_PT> states;
    find_all_dp_pts(states);
    all_dp_pts_isable = true;
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


int Square::self_cycle_check(const DP_PT &s0, const DP_PT &s1, const DP_PT &s2, const DP_PT &s3){
    assert(s0.is_self_cycle == true);
    if(s1.is_self_cycle || s2.is_self_cycle || s3.is_self_cycle){
        return false;
    }
    if(!s1.p.empty() || !s2.p.empty() || !s3.p.empty()){
        return false;
    }
    return true;
}


bool Square::allow_merge(const DP_PT &big, const DP_PT &s0){
    if(s0.is_self_cycle){
        // 小 portal 有 self cycle 則大 portal 則必定要是 self cycle
        return big.is_self_cycle;
    }
    
    Coord g = corner[Idx_LU];
    int cnt = 0;
    for(auto i:big.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = get_Portal_Coord(path.first);
        Coord b = get_Portal_Coord(path.second);
        if(a == g){
            cnt++;
        }
        if(b == g){
            cnt++;
        }
    }
    for(auto i:s0.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = get_Portal_Coord(path.first);
        Coord b = get_Portal_Coord(path.second);
        if(a == g){
            cnt++;
        }
        if(b == g){
            cnt++;
        }
    }
    return (cnt % 2) == 0;
}


bool Square::allow_merge(const DP_PT &big, const DP_PT &s0, const DP_PT &s1){
    if(s0.is_self_cycle){
        return big.is_self_cycle && self_cycle_check(s0, s1, s1, s1);
    }
    if(s1.is_self_cycle){
        return big.is_self_cycle && self_cycle_check(s1, s0, s0, s0);
    }
    
    vector<Coord> check_list;
    vector<int> cnt;
    check_list.emplace_back(corner[Idx_LD]);
    cnt.emplace_back(0);
    check_list.emplace_back(children[Idx_LD]->corner[Idx_LU]);
    cnt.emplace_back(0);

    for(auto i:big.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = get_Portal_Coord(path.first);
        Coord b = get_Portal_Coord(path.second);
        for(int j=0;j<(int)check_list.size();j++){
            Coord g = check_list[j];
            if(a == g){
                cnt[j]++;
            }
            if(b == g){
                cnt[j]++;
            }
        }
    }
    for(auto i:s0.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = get_Portal_Coord(path.first);
        Coord b = get_Portal_Coord(path.second);
        for(int j=0;j<(int)check_list.size();j++){
            Coord g = check_list[j];
            if(a == g){
                cnt[j]++;
            }
            if(b == g){
                cnt[j]++;
            }
        }
    }
    for(auto i:s1.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = get_Portal_Coord(path.first);
        Coord b = get_Portal_Coord(path.second);
        for(int j=0;j<(int)check_list.size();j++){
            Coord g = check_list[j];
            if(a == g){
                cnt[j]++;
            }
            if(b == g){
                cnt[j]++;
            }
        }
    }
    for(int e:cnt){
        if(e % 2 != 0){
            return false;
        }
    }
    return true;
}

bool Square::allow_merge(const DP_PT &big, const DP_PT &s0, const DP_PT &s1, const DP_PT &s2){
    if(s0.is_self_cycle){
        return big.is_self_cycle && self_cycle_check(s0, s1, s2, s1);
    }
    if(s1.is_self_cycle){
        return big.is_self_cycle && self_cycle_check(s1, s0, s2, s0);
    }
    if(s2.is_self_cycle){
        return big.is_self_cycle && self_cycle_check(s2, s0, s1, s1);
    }
    vector<Coord> check_list;
    vector<int> cnt;
    check_list.emplace_back(corner[Idx_RU]);
    cnt.emplace_back(0);
    check_list.emplace_back(children[Idx_RU]->corner[Idx_LU]);
    cnt.emplace_back(0);

    for(auto i:big.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = get_Portal_Coord(path.first);
        Coord b = get_Portal_Coord(path.second);
        for(int j=0;j<(int)check_list.size();j++){
            Coord g = check_list[j];
            if(a == g){
                cnt[j]++;
            }
            if(b == g){
                cnt[j]++;
            }
        }
    }
    for(auto i:s0.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = get_Portal_Coord(path.first);
        Coord b = get_Portal_Coord(path.second);
        for(int j=0;j<(int)check_list.size();j++){
            Coord g = check_list[j];
            if(a == g){
                cnt[j]++;
            }
            if(b == g){
                cnt[j]++;
            }
        }
    }
    for(auto i:s2.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = get_Portal_Coord(path.first);
        Coord b = get_Portal_Coord(path.second);
        for(int j=0;j<(int)check_list.size();j++){
            Coord g = check_list[j];
            if(a == g){
                cnt[j]++;
            }
            if(b == g){
                cnt[j]++;
            }
        }
    }
    for(int e:cnt){
        if(e % 2 != 0){
            return false;
        }
    }
    return true;
}


bool Square::allow_merge(const DP_PT &big, const DP_PT &s0,\
                         const DP_PT &s1, const DP_PT &s2, const DP_PT &s3){
    if(s0.is_self_cycle){
        return big.is_self_cycle && self_cycle_check(s0, s1, s2, s3);
    }
    if(s1.is_self_cycle){
        return big.is_self_cycle && self_cycle_check(s1, s0, s2, s3);
    }
    if(s2.is_self_cycle){
        return big.is_self_cycle && self_cycle_check(s2, s1, s0, s3);
    }
    if(s3.is_self_cycle){
        return big.is_self_cycle && self_cycle_check(s3, s1, s2, s0);
    }
    map<Coord, int> cnt_node_degree;
    map<Coord, vector<Coord>> adj;
    map<Coord, int> vis_big, vis_small;
    for(auto i:big.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = get_Portal_Coord(path.first);
        Coord b = get_Portal_Coord(path.second);
        cnt_node_degree[a]++;
        cnt_node_degree[b]++;
        vis_big[a] = -1;
        vis_big[b] = -1;
    }

    for(auto i:s0.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = children[0]->get_Portal_Coord(path.first);
        Coord b = children[0]->get_Portal_Coord(path.second);
        cnt_node_degree[a]++;
        cnt_node_degree[b]++;
        adj[a].emplace_back(b);
        adj[b].emplace_back(a);
        vis_small[a] = -1;
        vis_small[b] = -1;
    }

    for(auto i:s1.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = children[1]->get_Portal_Coord(path.first);
        Coord b = children[1]->get_Portal_Coord(path.second);
        cnt_node_degree[a]++;
        cnt_node_degree[b]++;
        adj[a].emplace_back(b);
        adj[b].emplace_back(a);
        vis_small[a] = -1;
        vis_small[b] = -1;
    }

    for(auto i:s2.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = children[2]->get_Portal_Coord(path.first);
        Coord b = children[2]->get_Portal_Coord(path.second);
        cnt_node_degree[a]++;
        cnt_node_degree[b]++;
        adj[a].emplace_back(b);
        adj[b].emplace_back(a);
        vis_small[a] = -1;
        vis_small[b] = -1;
    }

    for(auto i:s3.p){
        pair<Portal_id, Portal_id> path = portal_pairs[i.first];
        Coord a = children[3]->get_Portal_Coord(path.first);
        Coord b = children[3]->get_Portal_Coord(path.second);
        cnt_node_degree[a]++;
        cnt_node_degree[b]++;
        adj[a].emplace_back(b);
        adj[b].emplace_back(a);
        vis_small[a] = -1;
        vis_small[b] = -1;
    }

    for(auto i:cnt_node_degree){
        if(i.second & 1){
            return false;
        }
    }
        
    //檢查所有小 portal 都有連到至少一個 portal 連到
    queue<Coord> q;
    for(auto iter:vis_big){
        q.emplace(iter.first);
    }
    if(vis_big.size() == 0){
        q.emplace(vis_small.begin()->first);
    }
    Coord node;
    while(!q.empty()){
        node = q.front();
        q.pop();

        if(vis_small.find(node) != vis_small.end()){
            // 競程之尺
            vis_small[node] = 1;
        }
        
        for(auto iter:adj[node]){
            // 競程之恥
            auto tmp = vis_small.find(iter);
            if( tmp != vis_small.end() && tmp->second == -1){
                vis_small[iter] = 1;
                q.emplace(iter);
            }
        }
    }
    for(auto iter:vis_small){
        if(iter.second != 1){
            return false;
        }
    }

    return true;
}

void Square::random_delete(){
    //random 亂剪枝
    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<double> dis(0.0, 1.0);

    map<int, bool> random_dp_table;
    for(auto iter:dp_table){
        if(!iter.second){
            continue;
        }
        //保留含有 self cycle 的 state
        vector<DP_PT> states = all_dp_pts[iter.first];
        bool flag = false;
        for(int cycle=0;cycle<Parameter::k;cycle++){
            if(states[cycle].is_self_cycle){
                flag = true;
                break;
            }
        }
        
        if(flag || dis(gen) <= Parameter::random_keep_ratio){
            random_dp_table.insert(iter);
        }
    }
    swap(dp_table, random_dp_table);
}

map<int, bool> Square::get_dp_table(){
    cout << "Square::get_dp_table()" << endl;
    cout << "id " << id << endl;
    if(dp_table_isable){
        return dp_table;
    }
    dp_table.clear();
    if(node_list.size() == 0){
        cout << "node_list == 0" << endl;
        find_all_dp_pts();
        
        for(int state_id = 0;state_id < (int)all_dp_pts.size();state_id++){
            vector<DP_PT> states = all_dp_pts[state_id];
            bool is_good_state = true;
            for(DP_PT st:states){
                if(st.is_self_cycle){
                    is_good_state = false;
                    break;
                }
                double distance_sum = 0;
                bool flag = true;                   //eliminate self cycle
                for(auto it:st.p){
                    pair<Portal_id, Portal_id> last = portal_pairs[it.first];
                    Coord a = get_Portal_Coord(last.first);
                    Coord b = get_Portal_Coord(last.second);
                    if(a == b){
                        flag = false;
                        break;
                    }
                    for(int iter = 0; iter < it.second; iter++){
                        cout << "entry(x, y) = (" << a.x << ", " << a.y << "), exit(x, y) = (" << b.x << ", " << b.y << ")" << endl;
                    }
                    distance_sum += AlgorithmBase::distance(a, b) * it.second;
                }
                if(!flag || st.T_id >= (int)T.size() || distance_sum > T[st.T_id]){
                    is_good_state = false;
                    break;
                }
            }
            if(is_good_state){
                dp_table[state_id] = true;
            }
        }

        dp_table_isable = true;
        return dp_table;
    }
    if(node_list.size() == 1){
        cout << "node_list == 1" << endl;
        cout << "node(x, y) = (" << node_list[0].x << ", " << node_list[0].y << ")" << endl;
        find_all_dp_pts();

        for(int state_id = 0;state_id < (int)all_dp_pts.size();state_id++){
            vector<DP_PT> states = all_dp_pts[state_id];
            bool is_good_state = true;
            bool cycle_pass = false;                // exist any cycle can go through the internal node
            for(DP_PT st:states){
                if(st.is_self_cycle){
                    is_good_state = true;
                    continue;
                }
                double distance_sum = 0;
                bool flag = true;                   //eliminate self cycle
                int self_cycle = 0;
                for(auto it:st.p){
                    pair<Portal_id, Portal_id> last = portal_pairs[it.first];
                    Coord a = get_Portal_Coord(last.first);
                    Coord b = get_Portal_Coord(last.second);
                    for(int iter = 0; iter < it.second; iter++){
                        cout << "entry(x, y) = (" << a.x << ", " << a.y << "), exit(x, y) = (" << b.x << ", " << b.y << ")" << endl;
                    }
                    if(a == b){
                        self_cycle += 1;
                    }
                    distance_sum += AlgorithmBase::distance(a, b) * it.second;
                }
                double mi = 1e9;
                if(!flag || st.T_id >= (int)T.size() || distance_sum > T[st.T_id]){
                    is_good_state = false;
                    break;
                }
                if(self_cycle > 1){
                    is_good_state = false;
                    break;
                }
                if(self_cycle == 1){
                    for(auto it:st.p){
                        pair<Portal_id, Portal_id> last = portal_pairs[it.first];
                        Coord a = get_Portal_Coord(last.first);
                        Coord b = get_Portal_Coord(last.second);
                        if(a == b && a == node_list[0]){
                            // 由於我們允許不選任何 portal 配對，故該點可以獨立形成一個 cycle
                            // 這個如果不 break 可能導致一個 cycle 中有兩個獨立的 cycle
                            break;
                        }
                        if(a == b){
                            // 有自環則必定需經過 internal node，若沒有這個限制一樣會導致一個 cycle 中有兩個獨立的 cycle
                            double pass_node = AlgorithmBase::distance(a, node_list[0]) + AlgorithmBase::distance(b, node_list[0]);
                            mi = min(mi, distance_sum - AlgorithmBase::distance(a, b) + pass_node);
                            cout << "minimum distance = " << mi << endl;
                            if(st.T_id < (int)T.size() && mi <= T[st.T_id]){
                                cycle_pass = true;
                            }else{
                                is_good_state = false;
                            }
                            break;
                        }
                    }
                }else{
                    //self_cycle == 0
                    // 找任意 path 連到 internal node，並且檢查是否滿足長度限制
                    for(auto it:st.p){
                        pair<Portal_id, Portal_id> last = portal_pairs[it.first];
                        Coord a = get_Portal_Coord(last.first);
                        Coord b = get_Portal_Coord(last.second);
                        double pass_node = AlgorithmBase::distance(a, node_list[0]) + AlgorithmBase::distance(b, node_list[0]);
                        mi = min(mi, distance_sum - AlgorithmBase::distance(a, b) + pass_node);
                    }
                    cout << "minimum distance = " << mi << endl;
                    if(st.T_id < (int)T.size() && mi <= T[st.T_id]){
                        cycle_pass = true;
                    }
                }

            }
            if(is_good_state && cycle_pass){
                dp_table[state_id] = true;
            }
        }
        dp_table_isable = true;
        
        return dp_table;
    }

    // recursive
    vector<map<int, bool>> children_dp_table(4);
    // #pragma omp parallel for
    for(int i = 0;i < 4;i++){
        if(children[i] != nullptr){
            children_dp_table[i] = children[i]->get_dp_table();
        }
    }


    cout << "merge" << endl;
    // merge
    find_all_dp_pts();
    #pragma omp parallel for
    for(int state_id = 0;state_id < (int)all_dp_pts.size();state_id++){
        vector<DP_PT> states = all_dp_pts[state_id];
        bool is_good_state = true;
        for(auto p0:children_dp_table[0]){
            vector<DP_PT> states0 = all_dp_pts[p0.first];
            if(!p0.second){
                continue;
            }
            do{
                {
                    bool next = false;
                    for(int cycle=0;cycle<Parameter::k;cycle++){
                        if(!allow_merge(states[cycle], states0[cycle])){
                            next = true;
                            break;
                        }
                    }
                    if(next){
                        continue;
                    }
                }
                for(auto p1: children_dp_table[1]){
                    vector<DP_PT> states1 = all_dp_pts[p1.first];
                    if(!p1.second){
                        continue;
                    }
                    do{
                        {
                            bool next = false;
                            for(int cycle=0;cycle<Parameter::k;cycle++){
                                if(!allow_merge(states[cycle], states0[cycle], states1[cycle])){
                                    next = true;
                                    break;
                                }
                            }
                            if(next){
                                continue;
                            }
                        }
                        for(auto p2:children_dp_table[2]){
                            vector<DP_PT> states2 = all_dp_pts[p2.first];
                            if(!p2.second){
                                continue;
                            }
                            do{
                                {
                                    bool next = false;
                                    for(int cycle=0;cycle<Parameter::k;cycle++){
                                        if(!allow_merge(states[cycle], states0[cycle], states1[cycle], states2[cycle])){
                                            next = true;
                                            break;
                                        }
                                    }
                                    if(next){
                                        continue;
                                    }
                                }
                                for(auto p3:children_dp_table[3]){
                                    vector<DP_PT> states3 = all_dp_pts[p3.first];
                                    if(!p3.second){
                                        continue;
                                    }
                                    do{
                                        bool good_cycle = true;
                                        for(int cycle=0;cycle<Parameter::k;cycle++){
                                            if(allow_merge(states[cycle], states0[cycle], states1[cycle], states2[cycle], states3[cycle])){
                                                double distance_sum = children[0]->T[states0[cycle].T_id];
                                                distance_sum += children[1]->T[states1[cycle].T_id];
                                                distance_sum += children[2]->T[states2[cycle].T_id];
                                                distance_sum += children[3]->T[states3[cycle].T_id];
                                                if(T[states[cycle].T_id] < distance_sum){
                                                    good_cycle = false;
                                                }
                                            }
                                        }
                                        if(good_cycle){
                                            is_good_state = true;
                                        }
                                    }while(next_permutation(states3.begin(), states3.end()));
                                }
                            }while(next_permutation(states2.begin(), states2.end()));
                        }
                    }while(next_permutation(states1.begin(), states1.end()));
                    
                }
            }while(next_permutation(states0.begin(), states0.end()));
        }
        if(is_good_state){
            dp_table[state_id] = true;
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
        if(i.x - mid.x < EPS && i.y - mid.y > -EPS){
            v[Idx_LU].emplace_back(i);
        }
        else if(i.x - mid.x < EPS){
            v[Idx_LD].emplace_back(i);
        }else if(i.y - mid.y > -EPS){
            v[Idx_RU].emplace_back(i);
        }else{
            v[Idx_RD].emplace_back(i);
        }
    }
    children[Idx_LU] = new Square(corner[Idx_LU], mid, v[Idx_LU], this, algoptr, depth + 1);
    children[Idx_LU] -> id = algoptr -> get_counter(children[Idx_LU]);
    
    children[Idx_LD] = new Square(make_pair(corner[Idx_LD].x, mid.y), make_pair(mid.x, corner[Idx_LD].y), v[Idx_LD], this, algoptr, depth + 1);
    children[Idx_LD] -> id = algoptr -> get_counter(children[Idx_LD]);
    
    children[Idx_RU] = new Square(make_pair(mid.x, corner[Idx_RU].y), make_pair(corner[Idx_RU].x, mid.y), v[Idx_RU], this, algoptr, depth + 1);
    children[Idx_RU] -> id = algoptr -> get_counter(children[Idx_RU]);
    
    children[Idx_RD] = new Square(mid, corner[Idx_RD], v[Idx_RD], this, algoptr, depth + 1);
    children[Idx_RD] -> id = algoptr -> get_counter(children[Idx_RD]);

    for(int i = 0;i < 4;i++){
        children[i] -> make_tree_dfs();
    }
}


Square::Square(Coord _upleft, Coord _downright, vector<Coord> _node_list, Square *_parent, MyAlgo * ptr, int dep)
    :depth(dep), node_list(_node_list), parent(_parent), dp_table_isable(false), algoptr(ptr){
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
    // squares.emplace_back(this);
    T.clear();
    double L = algoptr -> get_L();
    L *= L;
    L /= (1 << depth);
    L /= Parameter::m + 1;
    L = min(L, algoptr -> get_scaled_input().get_B());
    double alpha = _upleft.y - _downright.y;
    while(alpha < L){
        T.emplace_back(alpha);
        alpha *= (1 + Parameter::epsilon_plum);
    }
    z = max(z, (int)T.size());
    all_dp_pts.clear();
    all_dp_pts_isable = false;
}


bool Square::is_leaf(){
    return node_list.size() <= 1;
}

void Square::set_id(int _id){
    id = _id;
}

Square::~Square(){
    for(int i = 0;i < 4;i++){
        if(children[i] != nullptr){
            delete children[i];
            children[i] = nullptr;
        }
    }
}
