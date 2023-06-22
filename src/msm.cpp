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

        int c_size = 4;
        int scalar_size = 32;
        int windows = scalar_size / c_size;


        /*
        Each b-bit (32-bit) scalar in V2 (scalars) will be decomposed into K c-bit (4-bit) scalars, where K = b / c.
        The value for c will be modified to maximize performance. Each K is referred to as a "window". Each
        window will have 2^c buckets, and the total number of buckets across all windows will be 2^c * K. For
        c = 4, each window will have 16 buckets, and a total of 128 buckets across K windows. Each bucket
        will have an index 0...15 in window K1, 16...31 in window K2, etc. 
        */

        vector<uint8_t> decomposedScalars;
  
        for (int i = 0; i < scalars.size(); i++) {
            for (int j = 0; j < c_size; j++) {
                uint8_t decomposedPart = (scalars[i] >> c_size*j) & 0xF;
                decomposedScalars.push_back(decomposedPart);
            }
        }

        

    }
};