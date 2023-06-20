#include "bh420.hpp"

class bn420 {
    //y^{2}+ a_{1}xy + a_{3}y = x^{3} + a_{2}x^{2} + a_{4}x + a_{6}

    private:
        int a1 = 0;
        int a2 = 6;
        int a3 = -10;
        int a4 = -10;
        int a6 = -10;

    public:
        bool &in(EC_point point) {
            int x = point.getX();
            int y = point.getY();
            return y*y+a1*x*y+a3*y == x*x*x+a2*x*x+a4*x+a6;
        }

};