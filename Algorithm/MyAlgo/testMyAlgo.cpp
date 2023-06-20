#include "MyAlgo.h"
using namespace std;

int main(){
    Input input(5);
    input.display();
    MyAlgo algo(input);
    algo.Rescale();
    // algo.display();
    algo.make_tree();
    algo.tree_travse();
    cout << "tsp ans: " << algo.get_dp_table().begin()->second << endl;
}