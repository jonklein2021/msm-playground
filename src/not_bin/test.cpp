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
        newPoint.print();
        points.push_back(newPoint);
    }
    
    // no more seg fault :) 
    Curve::EC_point addSimplePoint = MSM::addMethod(points, scalars);
    std::cout << "Result Point: ";
    addSimplePoint.print();

    
    Curve::EC_point doubleAddPoint = MSM::doubleAddMethod(points, scalars);
    std::cout << "Result Point: ";
    doubleAddPoint.print();

    return 0;
}