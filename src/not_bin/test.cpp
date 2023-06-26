#include <iostream>
#include <vector>
#include "msm.hpp"
#include <cmath>
#include <string.h>

#define NUM_TESTS 256

using namespace std;

int main(int argc, char** argv){
    cout << "RAHHHHH" << endl;

    if ((argc > 1) && (strcmp(argv[1], "test") == 0)) {
        // run tests
        cout << "Running " << NUM_TESTS << " tests...\n";
        for (size_t t = 0; t < NUM_TESTS; t++) {
            vector<Curve::EC_point> points;
            vector<unsigned> scalars;

            // generate a_i
            for (size_t k = 1; k <= 16; k++) {
                scalars.push_back(rand() % 1000);
            }

            // generate p_i
            for (size_t k = 1; k <= 16; k++) {
                double x = (rand() % 10000)/100.00;
                double y = sqrt(x * x * x + 6 * x * x - 10 * x + 15) + 5; //algebra works out to this, because a = 0
                Curve::EC_point newPoint(x, y);
                // newPoint.print();
                points.push_back(newPoint);
            }

            // ==================== DOUBLE-AND-ADD ====================
            Curve::EC_point doubleAddPoint = MSM::doubleAddMethod(points, scalars);
            doubleAddPoint.print();

            cout << "\t";
            
            // ==================== PIPPENGER ====================
            Curve::EC_point pippengerPoint = MSM::pippengerMethod(points, scalars);
            pippengerPoint.println();


        } 
        exit(0);
    }

    Curve::EC_point p(0.0, 0.0);
    p = Curve::EC_point::generatePoint(1.0);

    // Curve::EC_point p1 = p*7; // naive
    // Curve::EC_point p2 = p.times(7); // double and add
    // cout << "p*1 = "; p.print();
    // cout << "p*2 = "; (p*2).print();
    // cout << "p*3 = "; (p*3).print();
    // cout << "p*4 = "; (p*4).print();
    cout << "(p*2)+(p*2) = "; ((p*2)+(p*2)).print();
    cout << "(p*3)+(p*4) = "; ((p*3)+(p*4)).print();
    cout << "(p*4)+(p*4) = "; ((p*4)+(p*4)).print();
    cout << "(p*4)+(p*2) = "; ((p*4)+(p*2)).print();
    cout << "p.times(6) = "; p.times(5).print();   
    cout << "p*6 = "; (p*6).print();   
    // cout << "p*7 = "; (p*7).print();
    // cout << "p.times(7) = "; p.times(7).print();
    // cout << "p*8 = "; (p*8).print();
    // cout << "p.times(8) = "; p.times(8).print();
    // cout << "triple double = "; p.doublePoint().doublePoint().doublePoint().print();

    //y^{2}+ a_{1}xy + a_{3}y = x^{3} + a_{2}x^{2} + a_{4}x + a_{6}

    return 0;
}