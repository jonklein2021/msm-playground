#include "curve.hpp"
#include <iostream>
#include <cmath>

int Curve::a1 = 0;
int Curve::a2 = 6;
int Curve::a3 = -10;
int Curve::a4 = -10;
int Curve::a6 = -10;

Curve::EC_point::EC_point(double x0, double y0) {
    x = x0;
    y = y0;
}

const double Curve::EC_point::getX() {
    return x;
}

const double Curve::EC_point::getY() {
    return y;
}

bool Curve::EC_point::isValid() {
    return (y*y + a1*x*y + a3*y) == (x*x*x + a2*x*x + a4*x + a6);
}

void Curve::EC_point::print() {
    printf("(%.2f, %.2f)\n", x, y);
}

Curve::EC_point &Curve::EC_point::operator+(const Curve::EC_point a) {
    // point addition
    const double lambda = (a.x-x)/(a.y-y);
    const double newX = lambda*lambda-x-a.x;
    const double newY = lambda*(x-newX)-y;
    EC_point result(newX, newY);
    std::cout << "hola from +\n";
    return result;
}

Curve::EC_point &Curve::EC_point::operator+=(const Curve::EC_point a) {
    // point addition and assignment
    std::cout << "hola from +=\n";
    EC_point result = EC_point(x, y) + a; // seggy fault :(
    std::cout << "hola from +=\n";
    x = result.getX(); y = result.getY();
    return result;
}

/**
 * @brief naive single-scalar multipliation
 * 
 * @param n the scalar to multiply by
 * @return EC_point
 */
Curve::EC_point &Curve::EC_point::operator*(const unsigned n) {
    EC_point result = *this;
    for (size_t i = 0; i < n-1; i++) {
        result += result;
    }
    return result;
}

Curve::EC_point &Curve::EC_point::operator=(const Curve::EC_point point) {
    x = point.getX(); y = point.getY();
}

/**
 * @brief double-and-add single-scalar multipliation
 * 
 * @param n the scalar to multiply by
 * @return EC_point
 */
Curve::EC_point Curve::EC_point::times(const unsigned n) {
    if (n == 1) return *this;

    EC_point result(0.0, 0.0);
    unsigned remaining = n;
    while (remaining > 0) {
        unsigned x = 1;
        EC_point partial = *this;
        while ((x << 1) <= remaining) {
            x <<= 1;
            partial.doublePoint();
        }
        remaining -= x;
        result += partial;
    }
    
    return result;
}

/*
point doubling i.e. P + P = 2P requires:
lambda = (3x_1^2 + 2a_2x_1 - a_1y_1 + a_4) / (2y_1 + a_1x_1 + a_3)
we denote 2P (the result) as (x_3, y_3)
*/
Curve::EC_point Curve::EC_point::doublePoint() {
    const double lambda = (3 * (x * x) + 2 * a2 * x - a1 * y + a4) / (2 * y + a1 * x + a3);
    const double x3 = lambda * lambda + lambda * a1 - a2 - 2 * x;
    const double y3 = -1 * a1 * x3 - a3 - lambda * x3 + lambda * x - y;

    EC_point result(x3, y3);
    return result;
}

Curve::EC_point Curve::EC_point::generatePoint(double x) {
    double y = sqrt(x*x*x + 6*x*x - 10*x + 15) + 5;
    return EC_point(x, y);
}

void Curve::print() {
    printf("y^2 + %dxy + %dy = x^3 + %dx^2 + %dx + %d\n", a1, a3, a2, a4, a6);
}

