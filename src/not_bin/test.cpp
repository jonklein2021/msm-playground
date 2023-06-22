#include <iostream>
#include <vector>
#include "msm.hpp"
#include <cmath>

using namespace std;

int main(void){
    std::cout << "RAHHHHH" << std::endl;
    
    vector<Curve::EC_point> points;
    vector<unsigned> scalars;

    //y^{2}+ a_{1}xy + a_{3}y = x^{3} + a_{2}x^{2} + a_{4}x + a_{6}

    for (size_t k = 1; k <= 16; k++) {
        scalars.push_back(3);
    }

    for (size_t k = 1; k <= 16; k++) {
        double x = (rand() % 10000)/100.00;
        double y = sqrt(x * x * x + 6 * x * x - 10 * x + 15) + 5; //algebra works out to this, because a = 0
        Curve::EC_point newPoint(x, y);
        // newPoint.print();
        points.push_back(newPoint);
    }

    Curve::EC_point p(0.0, 0.0);
    p = Curve::EC_point::generatePoint(1.0);


    Curve::EC_point p1 = p*7; // naive
    Curve::EC_point p2 = p.times(7); // double and add
    cout << "1p = "; p.print();
    cout << "2p = "; (p*2).print();
    cout << "3p* = "; (p*3).print();
    cout << "4p = "; (p*4).print();
    cout << "7p# = "; ((p*3)+(p*4)).print();
    cout << "7p = "; (p*7).print();
    cout << "p2 = "; p2.print();
        
    exit(1);
    
    for (size_t i = 7; i <= 7; i++) {
        // cout << "i=" << i << " ";
        Curve::EC_point p1 = p*i; // naive
        Curve::EC_point p2 = p.times(i); // double and add
        cout << "p1 = "; p1.print();
        cout << "p2 = "; p2.print();
        if (p1 == p2) {
            cout << "pass\n";
        } else {
            // cout << "i=" << i << " ";
            cout << "fail\n";
        }
    }

    exit(1);
    
    // naive add
    cout << "==================== NAIVE ADDITION ====================\n";
    Curve::EC_point addSimplePoint = MSM::addMethod(points, scalars);
    std::cout << "Result Point: ";
    addSimplePoint.print();

    // for (size_t i = 0; i < 16; i++) {
    //     cout << scalars[i] << " ";
    //     points[i].print();
    // }

    // double-and-add
    cout << "==================== DOUBLE-AND-ADD ====================\n";
    Curve::EC_point doubleAddPoint = MSM::doubleAddMethod(points, scalars);
    std::cout << "Result Point: ";
    doubleAddPoint.print();

    
    // for (size_t i = 0; i < 16; i++) {
    //     cout << scalars[i] << " ";
    //     points[i].print();
    // }

    return 0;
}