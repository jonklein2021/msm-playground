#pragma once

class Curve {
    public:
        int a1, a2, a3, a4, a6;

        Curve(int b1, int b2, int b3, int b4, int b6);
        void print();

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
                EC_point generatePoint(double x);
                void print();
        };

};