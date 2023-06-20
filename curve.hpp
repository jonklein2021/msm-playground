class Curve {
    //y^{2}+ a_{1}xy + a_{3}y = x^{3} + a_{2}x^{2} + a_{4}x + a_{6}

    public:
        int a1, a2, a3, a4, a6;

        class EC_point {
            public:
                int x, y;
                EC_point(int a, int b);
                int getX();
                int getY();
                EC_point &operator+(const EC_point a);
                EC_point &operator*(const int a);
                EC_point doublePoint();
                bool isValid();
        };

};