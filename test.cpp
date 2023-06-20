#include <iostream>
#include <vector>
#include "curve.hpp"
#include <cmath>

using namespace std;

int main(void){
    std::cout << "RAHHHHH" << std::endl;
    
    vector<Curve::EC_point> points;
    vector<int> scalars;

    //y^{2}+ a_{1}xy + a_{3}y = x^{3} + a_{2}x^{2} + a_{4}x + a_{6}

    //y = sqrt{ x^{3} + a_{2}x^{2} + a_{4}x + a_{6} - a_{1}xy + a_{3}y }

    //

    for (int i = 1; i <= 16; i++) {
        scalars.push_back(2);
    }

    for (int x = 1; x <= 16; x++) {
        int y = sqrt(x * x * x + 6 * x * x - 10 * x + 15) + 5;
        EC_point newPoint = new EC_point(x, y);
        points.push_back(newPoint);
    }

    return 0;
}