#include <iostream>
#include <vector>
#include "msm.hpp"
#include <cmath>
#include <string.h>

#define NUM_TESTS 32
#define N 1

using namespace std;

int main(int argc, char** argv){
    std::cout << "Running " << NUM_TESTS << " tests...\n";

    vector<Curve::EC_point> points;
    vector<unsigned> scalars;

    for (size_t t = 0; t < NUM_TESTS; t++) {
        // generate scalars
        for (size_t k = 0; k < N-1; k++){
            scalars.push_back(rand() % UINT32_MAX);
        } scalars.push_back(UINT32_MAX);

        // generate points
        for (size_t k = 0; k < N; k++) {
            double x = (rand() % 10000)/100.00;
            Curve::EC_point newPoint = Curve::EC_point::generatePoint(x); //algebra works out to this, because a = 0
            points.push_back(newPoint);
        }
        
        // for (size_t i = 0; i < N; i++){
        //     cout << scalars[i] << " ";
        // }

        // for (size_t i = 0; i < N; i++){
        //     points[i].println();
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

    //y^{2}+ a_{1}xy + a_{3}y = x^{3} + a_{2}x^{2} + a_{4}x + a_{6}

    return 0;
}