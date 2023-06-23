#include "MyAlgo.h"
#include <sys/time.h>
using namespace std;



int main(){
    timeval start;
    timeval end;
    gettimeofday(&start, NULL);
    Input input;
    input.display();
    MyAlgo algo(input);
    double ans = algo.run();
    cout << "tsp ans: " << ans << endl;
    gettimeofday(&end, NULL);
    cout<<"run time: "<<(1000000LL * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000000.0<<"sec"<<endl;
}