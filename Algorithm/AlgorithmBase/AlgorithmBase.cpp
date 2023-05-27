#include    "AlgorithmBase.h"

AlgorithmBase::AlgorithmBase(const string &algo_name, const Input &_input)
    :algorithm_name(algo_name), input(_input){}

string AlgorithmBase::get_algo_name(){
    return algorithm_name; 
}

double AlgorithmBase::distance(Coord p1, Coord p2){
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}