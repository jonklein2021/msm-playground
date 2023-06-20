#include "curve.hpp"

class Curve {
    //y^{2}+ a_{1}xy + a_{3}y = x^{3} + a_{2}x^{2} + a_{4}x + a_{6}

    public:
        //the bn-420 curve default parameters: 0, 6, -10, -10, -10
        static int a1, a2, a3, a4, a6;

        Curve(int b1, int b2, int b3, int b4, int b6) {
            a1 = b1;
            a2 = b2;
            a3 = b3;
            a4 = b4;
            a6 = b6;
        }

        class EC_point {
            public:
                int x, y;
                
                EC_point(int a, int b) {
                    x = a;
                    y = b;
                }

                const int getX() {
                    return x;
                }

                const int getY() {
                    return y;
                }

                EC_point &operator+(const EC_point a) {
                    // point addition
                    int lambda, newX, newY;
                    lambda = (a.x-x)/(a.y-y);
                    newX = lambda*lambda-x-a.x;
                    newY = lambda*(x-newX)-y;
                    EC_point result(newX, newY);
                    return result;
                }

                EC_point &operator+=(const EC_point a) {
                    // point addition and assignment
                    EC_point result = *this + a;
                    *this = result;
                    return result;
                }

                EC_point &operator*(const unsigned n) {
                    EC_point result = *this;
                    for (size_t i = 0; i < n-1; i++) {
                        result += result;
                    }
                    return result;
                }

                /*
                point doubling i.e. P + P = 2P requires:
                lambda = (3x_1^2 + 2a_2x_1 - a_1y_1 + a_4) / (2y_1 + a_1x_1 + a_3)
                we denote 2P (the result) as (x_3, y_3)
                */
                EC_point doublePoint() {
                    int lambda, x3, y3;
                    lambda = (3 * (x * x) + 2 * a2 * x - a1 * y + a4) / (2 * y + a1 * x + a3);
                    x3 = lambda * lambda + lambda * a1 - a2 - 2 * x;
                    y3 = -1 * a1 * x3 - a3 - lambda * x3 + lambda * x - y;

                    EC_point newPoint(x3, y3);
                    return newPoint;
                }

                
                bool isValid() {
                    return (y*y + a1*x*y + a3*y) == (x*x*x + a2*x*x + a4*x + a6);
                }
        };

};