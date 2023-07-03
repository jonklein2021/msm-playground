#include <algorithm>
#include <iostream>
#include <vector>
#include "msm.hpp"
#include <cmath>
#include <string.h>

int main() {
    Curve::EC_point p(0.0, 0.0);
    p = Curve::EC_point::generatePoint(5.0);

    // Curve::EC_point p1 = p*7; // naive
    // Curve::EC_point p2 = p.times(7); // double and add
    // cout << "p*1 = "; p.print();
    // cout << "p*2 = "; (p*2).print();
    // cout << "p*3 = "; (p*3).print();
    // cout << "p*4 = "; (p*4).print();
    std::cout << "(p+p) = "; (p+p).print();
    std::cout << "(p.double()) = "; p.doublePoint().print();
    std::cout << "(p*2) = "; (p*2).print();
    std::cout << "(p.times(2)) = "; (p.times(2)).print();
    std::cout << "(p*3)+(p*4) = "; ((p*3)+(p*4)).print();
    std::cout << "(p*4)+(p*4) = "; ((p*4)+(p*4)).print();
    std::cout << "(p*4)+(p*2) = "; ((p*4)+(p*2)).print();
    std::cout << "p.times(6) = "; p.times(6).print();   
    std::cout << "p*6 = "; (p*6).print();   
    // cout << "p*7 = "; (p*7).print();
    // cout << "p.times(7) = "; p.times(7).print();
    // cout << "p*8 = "; (p*8).print();
    // cout << "p.times(8) = "; p.times(8).print();
    // cout << "triple double = "; p.doublePoint().doublePoint().doublePoint().print();
}