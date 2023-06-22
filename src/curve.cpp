#include "curve.hpp"
#include <iostream>
#include <assert.h>
#include <float.h>
#include <cmath>

int Curve::a1 = 0;
int Curve::a2 = 6;
int Curve::a3 = -10;
int Curve::a4 = -10;
int Curve::a6 = -10;

Curve::EC_point::EC_point(double x0, double y0) {
    x = x0;
    y = y0;
    isInfinity = false;
}

Curve::EC_point::EC_point() {
    x = y = NAN;
    isInfinity = true;
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

bool Curve::EC_point::operator==(Curve::EC_point a) {
    return abs(x - a.x) < DBL_EPSILON && abs(y - a.y) < DBL_EPSILON;
}

bool Curve::EC_point::operator!=(Curve::EC_point a) {
    return !(abs(x - a.x) < DBL_EPSILON && abs(y - a.y) < DBL_EPSILON);
}

// point addition
Curve::EC_point Curve::EC_point::operator+(Curve::EC_point a) {
    // std::cout << "hola from +\n";

    if (this->isInfinity && a.isInfinity) { // O+O
        return EC_point();
    }

    if (this->isInfinity && !a.isInfinity) { // O+P
        this->isInfinity = false;
        return a;
    }

    if (!this->isInfinity && a.isInfinity) { // P+O
        return *this;
    }

    if (*this == a) return doublePoint(); // addition to itself
    const double lambda = (a.y - y)/(a.x - x);
    const double newX = lambda*lambda + a1*lambda - a2 - x - a.x;
    const double newY = -1*a1*newX - a3 - lambda*newX + lambda*x - y;
    return EC_point(newX, newY);
}

Curve::EC_point &Curve::EC_point::operator+=(Curve::EC_point a) {
    // point addition and assignment
    // std::cout << "hola from +=\n";

    if (this->isInfinity && !a.isInfinity) // O+P
        this->isInfinity = false;

    *this = *this + a;
    return *this;
}

/**
 * @brief naive single-scalar multipliation
 * 
 * @param n the scalar to multiply by
 * @return EC_point
 */
Curve::EC_point Curve::EC_point::operator*(unsigned n) {
    EC_point result(x, y);
    for (size_t i = 0; i < n-1; i++) {
        // std::cout <<  "hola from *\n";
        result += *this;
    }
    return result;
}

Curve::EC_point &Curve::EC_point::operator=(Curve::EC_point point) {
    // std::cout <<  "hola from =\n";
    x = point.getX();
    y = point.getY();
    isInfinity = point.isInfinity;
}

/**
 * @brief double-and-add single-scalar multipliation
 * 
 * @param n the scalar to multiply by
 * @return EC_point
 */
// Curve::EC_point Curve::EC_point::times(unsigned n) {
//     std::cout << "====================\n";
//     std::cout << "n=" << n << "\n";
//     if (n == 1) return *this;
//     if (n == 2) return doublePoint();

//     EC_point result(x, y);
//     unsigned remaining = n-1;
//     while (remaining > 0) {
//         unsigned k = 1;
//         EC_point partial(x, y);
//         while ((k << 1) <= remaining) {
//             k <<= 1;
//             partial = partial.doublePoint();
//         }
//         remaining -= k;
//         result += partial;
//         std::cout << k << " ";
//     }

//     std::cout << std::endl;
    
//     return result;
// }
Curve::EC_point Curve::EC_point::times(unsigned n) {
    std::cout << "====================\n";
    std::cout << "n=" << n << "\n";
    if (n == 1) return *this;
    if (n == 2) return doublePoint();

    EC_point result{};
    EC_point temp(x, y);
    
    std::cout << "initial = "; temp.print();

    while (n > 0) {
        unsigned long long b = n & 1;
        // printf("%llx", b);
        if (b == 1) {
            result += temp;
            std::cout << "temp = "; temp.print();
            std::cout << "result = "; result.print();
        }
        temp = temp.doublePoint();
        n >>= 1;
    }

    result.print();
    
    return result;
}

/*
point doubling i.e. P + P = 2P requires:
lambda = (3x_1^2 + 2a_2x_1 - a_1y_1 + a_4) / (2y_1 + a_1x_1 + a_3)
we denote 2P (the result) as (x_3, y_3)
*/
Curve::EC_point Curve::EC_point::doublePoint() {
    // std::cout << "hola from doublePoint()\n";
    const double lambda = (3*x*x + 2*a2*x - a1*y + a4)/(2*y + a1*x + a3);
    const double x3 = lambda*lambda + lambda*a1 - a2 - 2*x;
    const double y3 = -1*a1*x3 - a3 - lambda*x3 + lambda*x - y;
    return EC_point(x3, y3);
}

Curve::EC_point Curve::EC_point::generatePoint(double x) {
    double y = sqrt(x*x*x + 6*x*x - 10*x + 15) + 5;
    return EC_point(x, y);
}

void Curve::print() {
    printf("y^2 + %dxy + %dy = x^3 + %dx^2 + %dx + %d\n", a1, a3, a2, a4, a6);
}

