#include <iostream>
#include <vector>
#include "msm.hpp"
#include <cmath>
#include <string.h>

#define NUM_TESTS 32
#define N 16

using namespace std;

int main(int argc, char** argv){
    std::cout << "Running " << NUM_TESTS << " tests...\n";

    vector<Curve::EC_point> points;
    vector<unsigned> scalars;

    for (size_t t = 0; t < NUM_TESTS; t++) {
        // generate scalars
        for (size_t k = 0; k < N-1; k++){
            scalars.push_back(rand() % UINT32_MAX);
        } scalars.push_back(UINT32_MAX); // push this to make things interesting

        // generate points
        for (size_t k = 0; k < N; k++) {
            double x = (rand() % 10000)/100.00;
            Curve::EC_point newPoint = Curve::EC_point::generatePoint(x);
            points.push_back(newPoint);
        }

        cout.flush();

        // ================== DOUBLE-AND-ADD ===================
        Curve::EC_point doubleAddPoint = MSM::doubleAddMethod(points, scalars);
        doubleAddPoint.print();

        std::cout << "\t";
        
        // ==================== PIPPENGER ====================
        Curve::EC_point pippengerPoint = MSM::pippengerMethod(points, scalars);
        pippengerPoint.println();

        // reset points and scalars for next iteration
        points.clear();
        scalars.clear();
    }

    return 0;
}