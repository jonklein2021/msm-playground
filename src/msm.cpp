#include "curve.hpp"
#include <assert.h>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

namespace MSM {
    Curve::EC_point addMethod(vector<Curve::EC_point> points, vector<unsigned> scalars) {
        assert(points.size() == scalars.size());
        Curve::EC_point resultPoint(0.0, 0.0);
        for (size_t i = 0; i < points.size(); i++) {
            // (points[i] * scalars[i]).print();
            resultPoint += points[i] * scalars[i];
        }
        return resultPoint;
    }

    Curve::EC_point doubleAddMethod(vector<Curve::EC_point> points, vector<unsigned> scalars) {
        assert(points.size() == scalars.size());
        Curve::EC_point resultPoint(0.0, 0.0);
        for (size_t i = 0; i < points.size(); i++) {
            // points[i].times(scalars[i]).print();
            resultPoint += points[i].times(scalars[i]);
        }
        return resultPoint;
    }

    Curve::EC_point pippengerMethod(vector<Curve::EC_point> points, vector<unsigned> scalars) {
        assert(points.size() == scalars.size());
        Curve::EC_point resultPoint(0, 0);

        const unsigned n = scalars.size();
        const unsigned c_size = 4;
        const unsigned scalar_size = 32;
        const unsigned windows = scalar_size / c_size;
        const unsigned mask = (1 << c_size) - 1; // 00...011...11 with windows bits; 0xF for c_size = 4


        /*
        Each b-bit (32-bit) scalar in V2 (scalars) will be decomposed into K c-bit (4-bit) scalars, where K = b / c.
        The value for c will be modified to maximize performance. Each K is referred to as a "window". Each
        window will have 2^c buckets, and the total number of buckets across all windows will be 2^c * K. For
        c = 4, each window will have 16 buckets, and a total of 128 buckets across K windows. Each bucket
        will have an index 0...15 in window K1, 16...31 in window K2, etc. 
        */

        vector<uint8_t> decomposedScalars; // matrix of a_ij scalars; ith row -> ith original scalar, jth column -> jth decomposition
  
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < c_size; j++) {
                uint8_t a_ij = (scalars[i] >> c_size*j) & mask;
                decomposedScalars.push_back(a_ij); // push each a_ij scalar component
            }
        }

        // compute each T_i = a_i*G_j
        
        struct calebStruct {
            int scalar;
            char* pointer;
        };

    }
};