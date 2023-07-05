#pragma once

// Curve is not currently defined over a finite field in this implementation
class Curve {
    public:
        static int a1, a2, a3, a4, a6;
        void print();

        // jon remark: EC_point should be a separate class, not an inner class within the Curve class
        class EC_point {
            private:
                double x, y;
            public:
                bool isInfinity;

                EC_point(); // for points at infinity
                EC_point(double x0, double y0);
                const double getX();
                const double getY();
                bool isValid();
                void print();
                void println();

                bool operator==(EC_point a);
                bool operator!=(EC_point a);
                EC_point operator+(EC_point a);
                EC_point operator+=(EC_point a);
                EC_point operator=(Curve::EC_point point);
                EC_point operator*(unsigned n); // naive ssm
                EC_point times(unsigned n); // double-and-add ssm
                EC_point doublePoint();
                static EC_point generatePoint(double x);
        };
};
