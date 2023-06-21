#include "curve.hpp"
#include <assert.h>
#include <vector>
#include <iostream>

using namespace std;

namespace MSM {
    Curve::EC_point addMethod(vector<Curve::EC_point> points, vector<unsigned> scalars) {
        assert(points.size() == scalars.size());
        Curve::EC_point resultPoint(0.0, 0.0);
        for (size_t i = 0; i < points.size(); i++) {
            // cout << "i=" << i << endl;
            resultPoint += points[i] * scalars[i];
        }
        return resultPoint;
    }

    Curve::EC_point doubleAddMethod(vector<Curve::EC_point> points, vector<unsigned> scalars) {
        assert(points.size() == scalars.size());
        Curve::EC_point resultPoint(0.0, 0.0);
        for (size_t i = 0; i < points.size(); i++) {
            resultPoint += points[i].times(scalars[i]);
        }
        return resultPoint;
    }

    Curve::EC_point pippengerMethod(vector<Curve::EC_point> points, vector<unsigned> scalars) {
        assert(points.size() == scalars.size());
        Curve::EC_point resultPoint(0, 0);

        /*
        Each b-bit (254-bit) scalar in V2 (scalars) will be decomposed into K c-bit (10-bit) scalars, where K = b / c.
        The value for c will be modified to maximize performance. Each K is referred to as a “window”. Each
        window will have 2c buckets, and the total number of buckets across all windows will be 2c * K. For
        c = 10, each window will have 1024 buckets, and a total of 26624 buckets across K windows. Each bucket
        will have an index 0...1023 in window K1, 1024...2047 in window K2, etc. 

        We are going to use 8 bit scalars, so K = 32/8 = 4 windows. Each window will have 2 * 8 = 16 buckets, for
         a total of 16 * 4 = 64 buckets across all windows. 
        */



    }
};