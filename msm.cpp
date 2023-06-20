#include "curve.hpp"
#include <assert.h>
#include <vector>

using namespace std;

class MSM {
    public:
        Curve::EC_point addMethod(vector<Curve::EC_point> points, vector<unsigned> scalars) {
            assert(points.size() == scalars.size());
            Curve::EC_point resultPoint(0, 0);
            for (size_t i = 0; i < points.size(); i++) {
                resultPoint += points[i] * scalars[i];
            }
            return resultPoint;
        }

        Curve::EC_point doubleAddMethod(vector<Curve::EC_point> points, vector<unsigned> scalars) {
            assert(points.size() == scalars.size());
            Curve::EC_point resultPoint(0, 0);
            for (size_t i = 0; i < points.size(); i++) {
                resultPoint += points[i].times(scalars[i]);
            }
            return resultPoint;
        }
};