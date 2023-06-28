#include <algorithm>
#include <iostream>
#include <vector>
#include "msm.hpp"
#include <cmath>
#include <string.h>

#define NUM_TESTS 1
#define N 16

using namespace std;

int main(int argc, char** argv){
    std::cout << "RAHHHHH" << endl;

    if ((argc > 1) && (strcmp(argv[1], "test") == 0)) {
        // run tests
        std::cout << "Running " << NUM_TESTS << " tests...\n";

        vector<Curve::EC_point> points;
        vector<unsigned> scalars;

        for (size_t t = 0; t < NUM_TESTS; t++) {
            // generate scalars
            for (size_t k = 0; k < N-1; k++){
                scalars.push_back(rand() % 0xFFFFFFFF);
            } scalars.push_back(UINT32_MAX);
            // generate(scalars.begin(), scalars.begin()+N, []() { return rand() % 1000; });

            // generate points
            for (size_t k = 0; k < N; k++) {
                double x = (rand() % 10000)/100.00;
                Curve::EC_point newPoint = Curve::EC_point::generatePoint(x); //algebra works out to this, because a = 0
                points.push_back(newPoint);
            }
            
            // for (size_t i = 0; i < N; i++){
            //     cout << scalars[i] << " "; points[i].println();
            // }

            cout.flush();

            // ================== DOUBLE-AND-ADD ===================
            Curve::EC_point doubleAddPoint = MSM::doubleAddMethod(points, scalars);
            doubleAddPoint.print();

            std::cout << "\t";
            
            // ==================== PIPPENGER ====================
            Curve::EC_point pippengerPoint = MSM::pippengerMethod(points, scalars);
            pippengerPoint.println();

            // reset for next iteration
            points.clear();
            scalars.clear();
        } 
        std::exit(0);
    }

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

    //y^{2}+ a_{1}xy + a_{3}y = x^{3} + a_{2}x^{2} + a_{4}x + a_{6}

    return 0;
}