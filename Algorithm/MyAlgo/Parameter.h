#ifndef __PARAMETER_H
#define __PARAMETER_H

class Parameter{
public:
    const int m;
    const int r;
    const double epsilon;
    const double k;
    Parameter()
        :m(2), r(3), epsilon(0.5), k(4){}
};

#endif