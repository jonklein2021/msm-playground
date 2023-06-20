class Curve {
    public:
        int a1, a2, a3, a4, a6;

        class EC_point {
            public:
                int x, y;
                EC_point(int a, int b);
                int getX();
                int getY();
                EC_point &operator+(const EC_point a);
                EC_point &operator+=(const EC_point a);
                EC_point &operator*(const unsigned n);
                EC_point times(const unsigned n);
                EC_point doublePoint();
                bool isValid();
        };

        void print();

};