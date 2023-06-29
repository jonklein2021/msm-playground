#pragma once

#include "curve.hpp"
#include <vector>

using namespace std;

namespace MSM {
    Curve::EC_point addMethod(vector<Curve::EC_point> points, vector<unsigned> scalars);
    Curve::EC_point doubleAddMethod(vector<Curve::EC_point> points, vector<unsigned> scalars);
    Curve::EC_point pippengerMethod(vector<Curve::EC_point> points, vector<unsigned> scalars);
};