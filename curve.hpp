class Curve {
    //y^{2}+ a_{1}xy + a_{3}y = x^{3} + a_{2}x^{2} + a_{4}x + a_{6}

    public:
        int a1;
        int a2;
        int a3;
        int a4;
        int a6;

        class EC_point {
            private:
                int x, y;

            public:
                EC_point(int a, int b);
                int getX();
                int getY();
                EC_point &operator+(const EC_point a);
                EC_point doublePoint(const EC_point a);
        };

        bool &in(EC_point point);
};