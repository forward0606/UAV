#include"MyAlgo.h"
using namespace std;

int main(){
    Input input(5);
    MyAlgo algo(input);
    algo.Rescale();
    algo.display();
    algo.make_tree();
    algo.tree_travse();
}