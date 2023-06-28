#include "curve.hpp"
#include <assert.h>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

namespace MSM {
    // doesnt work because naive multiplication doesnt work??
    Curve::EC_point addMethod(vector<Curve::EC_point> points, vector<unsigned> scalars) {
        assert(points.size() == scalars.size());
        Curve::EC_point resultPoint(0.0, 0.0);
        for (size_t i = 0; i < points.size(); i++) {
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
     * @brief "naive" pippenger algorithm, simplest implementation (bucket method)
     *      WE BUCKET METHOD IN THIS BITCH TAKE YO ASS BACK TO NAIVE IMPLEMENTATION
     * 
     * @param points 
     * @param scalars 
     * @return Curve::EC_point 
     */
    Curve::EC_point pippengerMethod(vector<Curve::EC_point> points, vector<unsigned> scalars) {
        cout << "Pippenger: Initiializing..." << endl;
        assert(points.size() == scalars.size());
        
        const unsigned n = scalars.size();
        const unsigned b = 32, c = 4, k = b/c;
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

        cout << "Pippenger: Decomposing scalars..." << endl;
        for (auto &&s : scalars) {
            printf("0x%x ", s);
        } cout << endl;
        

        for (size_t j = 0; j < k; j++) { // outer loop: iterate over components
            cout << "j=" << j << " ";
            for (size_t i = 0; i < n; i++) { // inner loop: iterate over scalars
                uint8_t a_ij = (scalars[i] >> c*j) & mask; // jth component of a_i
                scalarComps.push_back(a_ij); // a[i*n+j]
                printf("%x ", a_ij);
            }
            cout << endl;
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
        
        vector<vector<Curve::EC_point>> jonsBuckets(mask); // jon (evil vector)
        vector<Curve::EC_point> jawns; // jawns

        for (size_t j = 0; j < k; j++) { // iterate over windows
            cout << "j=" << j << endl;

            cout << "Pippenger: Accumulating buckets..." << endl;
            // accumulate points into buckets
            for (size_t i = 0; i < n; i++) { // iterate over scalars
                fflush(stdout);
                if (scalarComps[j*k+i] != 0) {
                    // add point to correct bucket (ith scalar corresponds to ith point)
                    jonsBuckets[scalarComps[j*k+i]-1].push_back(points[i]);
                }
            } //<---- when this loop is done, the scalars associated with the jth window have been placed
            
            cout << "Pippenger: Aggregating points in each bucket..." << endl;
            // aggregate points in each bucket via naive addition
            for (size_t b = 0; b < mask; b++) { // for each bucket, store sum of points in first index
                vector<Curve::EC_point> currentBucket = jonsBuckets[b];
                size_t bucketSize = currentBucket.size();
                for (size_t l = 1; l < bucketSize; l++) {
                    currentBucket[0] += currentBucket[l];
                }
            }

            cout << "Triangle sum initializing..." << endl;
            // aggregate buckets for this window via triangle sum and store result
            Curve::EC_point bucketAgg, prev;
            bucketAgg = prev = jonsBuckets[0][0];
            for (size_t b = mask-1; b >= 0; b--) {
                fflush(stdout);
                cout << "b=" << b << " ";
                printf("%x\n", jonsBuckets[b][0]);
                prev += jonsBuckets[b][0];
                bucketAgg += prev;
                cout << "hola from end" << endl;
            } 

            cout << "Pushing back jawns..." << endl;
            jawns.push_back(bucketAgg);

            cout << "Clearing buckets..." << endl;
            // clear set of buckets for next window
            for (auto &bucket : jonsBuckets) {
                bucket.clear();
            }
            
            cout << "HUGE" << endl;
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

        /*
        Algo:
        for each i in totalBuckets:
            Curve::EC_point tempPoint;
            for each currElement.size() j to 0: 
                tempPoint += currElement[j];
            currElement[0] = tempPoint;

        After algo completes we have each first element in totalBuckets[i][0] = S_1, S_2, ... S_k
        */

        Curve::EC_point finalGigaChadPoint{};

        cout << "Pippenger: Aggregating windows..." << endl;
        for (auto &&jawn : jawns) {
            finalGigaChadPoint += jawn;
        }
        
        return finalGigaChadPoint;

        // ⠀⠀⠘⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡜⠀⠀⠀
        // ⠀⠀⠀⠑⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡔⠁⠀⠀⠀
        // ⠀⠀⠀⠀⠈⠢⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠴⠊⠀⠀⠀⠀⠀
        // ⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⢀⣀⣀⣀⣀⣀⡀⠤⠄⠒⠈⠀⠀⠀⠀⠀⠀⠀⠀
        // ⠀⠀⠀⠀⠀⠀⠀⠘⣀⠄⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        // ⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠛⠋⠉⠈⠉⠉⠉⠉⠛⠻⢿⣿⣿⣿⣿⣿⣿⣿
        // ⣿⣿⣿⣿⣿⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⢿⣿⣿⣿⣿
        // ⣿⣿⣿⣿⡏⣀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿
        // ⣿⣿⣿⢏⣴⣿⣷⠀⠀⠀⠀⠀⢾⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿
        // ⣿⣿⣟⣾⣿⡟⠁⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣷⢢⠀⠀⠀⠀⠀⠀⠀⢸⣿
        // ⣿⣿⣿⣿⣟⠀⡴⠄⠀⠀⠀⠀⠀⠀⠙⠻⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⣿
        // ⣿⣿⣿⠟⠻⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠶⢴⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⣿
        // ⣿⣁⡀⠀⠀⢰⢠⣦⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⡄⠀⣴⣶⣿⡄⣿
        // ⣿⡋⠀⠀⠀⠎⢸⣿⡆⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⠗⢘⣿⣟⠛⠿⣼
        // ⣿⣿⠋⢀⡌⢰⣿⡿⢿⡀⠀⠀⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣧⢀⣼
        // ⣿⣿⣷⢻⠄⠘⠛⠋⠛⠃⠀⠀⠀⠀⠀⢿⣧⠈⠉⠙⠛⠋⠀⠀⠀⣿⣿⣿⣿⣿
        // ⣿⣿⣧⠀⠈⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠟⠀⠀⠀⠀⢀⢃⠀⠀⢸⣿⣿⣿⣿
        // ⣿⣿⡿⠀⠴⢗⣠⣤⣴⡶⠶⠖⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡸⠀⣿⣿⣿⣿
        // ⣿⣿⣿⡀⢠⣾⣿⠏⠀⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠉⠀⣿⣿⣿⣿
        // ⣿⣿⣿⣧⠈⢹⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿
        // ⣿⣿⣿⣿⡄⠈⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣾⣿⣿⣿⣿⣿
        // ⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿
        // ⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
        // ⣿⣿⣿⣿⣿⣦⣄⣀⣀⣀⣀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
        // ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
        // ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠙⣿⣿⡟⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿
        // ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠁⠀⠀⠹⣿⠃⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿
        // ⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⢐⣿⣿⣿⣿⣿⣿⣿⣿⣿
        // ⣿⣿⣿⣿⠿⠛⠉⠉⠁⠀⢻⣿⡇⠀⠀⠀⠀⠀⠀⢀⠈⣿⣿⡿⠉⠛⠛⠛⠉⠉
        // ⣿⡿⠋⠁⠀⠀⢀⣀⣠⡴⣸⣿⣇⡄⠀⠀⠀⠀⢀⡿⠄⠙⠛⠀⣀⣠⣤⣤⠄⠀
    }
};