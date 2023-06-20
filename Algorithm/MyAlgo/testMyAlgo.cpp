#include "MyAlgo.h"
#include <sys/time.h>
using namespace std;

int main(){
    timeval start;
    timeval end;
    gettimeofday(&start, NULL);
    Input input(5);
    input.display();
    MyAlgo algo(input);
    algo.Rescale();
    // algo.display();
    algo.make_tree();
    algo.tree_travse();
    algo.get_dp_table();
    cout << "tsp ans: " << algo.get_dp_table().begin()->second << endl;
    gettimeofday(&end, NULL);
    cout<<"run time: "<<(1000000LL * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000000.0<<"sec"<<endl;
}