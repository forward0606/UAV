#ifndef __PARAMETER_H
#define __PARAMETER_H

class Parameter{
public:
    const int m;
    const int r;
    const double epsilon;
    const double k;
    Parameter()
        :m(0), r(2), epsilon(1.0), k(4){}
};

#endif