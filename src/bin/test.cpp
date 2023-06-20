#include <iostream>
#include <vector>
#include "msm.hpp"
#include <cmath>

using namespace std;

int main(void){
    std::cout << "RAHHHHH" << std::endl;

    Curve bn_420(0, 6, -10, -10, -10);
    bn_420.print();
    
    vector<Curve::EC_point> points;
    vector<unsigned> scalars;

    //y^{2}+ a_{1}xy + a_{3}y = x^{3} + a_{2}x^{2} + a_{4}x + a_{6}

    for (size_t i = 1; i <= 16; i++) {
        scalars.push_back(2);
    }

    for (size_t x = 1; x <= 16; x++) {
        double y = sqrt(x * x * x + 6 * x * x - 10 * x + 15) + 5; //algebra works out to this, because a = 0
        Curve::EC_point newPoint(x, y);
        newPoint.print();
        points.push_back(newPoint);
    }

    std::cout << "\n\nResult Point:\n" << std::endl;
    
    Curve::EC_point addSimplePoint = MSM::addMethod(points, scalars);
    
    addSimplePoint.print();

    return 0;
}