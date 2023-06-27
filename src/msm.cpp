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
        size_t n = points.size();
        Curve::EC_point resultPoint(0.0, 0.0);
        for (size_t i = 0; i < n; i++) {
            resultPoint += points[i].times(scalars[i]);
        }
        return resultPoint;
    }

    /**
     * @brief "naive" pippenger algorithm, simplest implementation (not bucket method)
     * 
     * @param points 
     * @param scalars 
     * @return Curve::EC_point 
     */
    Curve::EC_point pippengerMethod(vector<Curve::EC_point> points, vector<unsigned> scalars) {
        cout << "hola from pippenger" << endl;
        assert(points.size() == scalars.size());
        
        const unsigned n = scalars.size();
        const unsigned b = 32;
        const unsigned c = 4;
        const unsigned k = b/c;
        const unsigned mask = (1 << c) - 1; // 00...011...11 with c 1s; 0xF for c = 4

        /*
        Each b-bit (32-bit) scalar in V2 (scalars) will be decomposed into K c-bit (4-bit) scalars, where K = b / c.
        The value for c will be modified to maximize performance. Each K is referred to as a "window". Each
        window will have 2^c buckets, and the total number of buckets across all k will be 2^c * K. For
        c = 4, each window will have 16 buckets, and a total of 128 buckets across K k. Each bucket
        will have an index 0...15 in window K1, 16...31 in window K2, etc. 
        */

        // matrix of a_ij scalars
        // ith row -> ith scalar, jth column -> jth decomposition of that scalar
        vector<unsigned> scalarComps;
        vector<Curve::EC_point> partialSums;

        for (size_t j = 0; j < k; j++) { // outer loop: iterate over components
            for (size_t i = 0; i < n; i++) { // inner loop: iterate over scalars
                uint8_t a_ij = (scalars[i] >> c*j) & mask; // jth component of a_i
                scalarComps.push_back(a_ij); // a[i*n+j]
                // cout << i << endl;
            }
            // partialSums[j] = MSM::doubleAddMethod(points, scalarComps);
            // scalarComps.clear();
        }

        /*
                      | a_1,1 ... a_1,k | <-- different window of same scalar
                      |  .           .  |
        scalarComps = |  .           .  |
                      |  .           .  |
                      | a_n,1 ... a_n,k |
                         ^
                         |-- same window of different scalars
        */
        
        /*
        Now that we have decomposed our scalars into c-bit components of each [ a_i,1 a_i,2, ... a_i,n ] 
        We need to:
            1. Map points to their correct scalar windows
            2. Add all points in a certain bucket, associated with certain windows
            3. Add all buckets together

        The MSM will then be completed.

        To map points to corresponding scalar windows, we will simply create an array of pairings:
        [[G_i],[k_i_j]], [[G_{i + 1}],[k_{i_{j + 1}]]] (2D ARRAY), a list of pairings, where each
        pairing is an array of size two of format (Point, Scalar Comp)
        */

        vector<vector<Curve::EC_point>> totalBuckets; //buckets are going to be vector's of EC points (elements of G)
        // totalBuckets.reserve(k); //allocate k elements for each window, which will be populated with 2^c - 1 buckets

        size_t sizeBuckets = totalBuckets.size();
        size_t sizeDecompScalars = scalarComps.size();

        /*
        -Iterate over the decomposed scalars vector
        -Divide current index by k and floor it to get the proper point location of the point vector
        -Put the point into the proper bucket, designated by currentDecomp
        */

        for (int i = 0; i < sizeDecompScalars; i++) {
            unsigned currentDecomp = scalarComps[i];
            vector<Curve::EC_point> currVec = totalBuckets[currentDecomp];
            int pointsIndex = i / k;

            currVec.push_back(points[pointsIndex]);

            totalBuckets.insert(totalBuckets.begin(), currentDecomp, currVec);
        }

        /*
        Now that the buckets have been filled, we can perform a triangle sum:
        S_k +
        S_k + S_k-1 +
        ...
        S_k + S_k-1 + ... + S_2
        S_k + S_k-1 + ... + S_2 + S_1
        = S_1 + S_2 + ... + (k-1)*(S_k-1) + k*S_k
        */

        Curve::EC_point result = partialSums[0];
        // for (size_t j = 1; j < k; j++) {
        //     result = result.times(1 << c);
        //     result += partialSums[j];
        // }
        
        return result;
    }
};