#include "curve.hpp"
#include <iostream>
#include <assert.h>
#include <float.h>
#include <cmath>

int Curve::a1 = 0;
int Curve::a2 = 0;
int Curve::a3 = 0;
int Curve::a4 = 0;
int Curve::a6 = 3;

//so a1 = 0, a3 = 0, a2 =0, a4 = 0, a6 = 3 for BN-254 Y^2 = X^3 + 3 with
//q = 21888242871839275222246405745257275088696311157297823662689037894645226208583

Curve::EC_point::EC_point(double x0, double y0) {
    x = x0;
    y = y0;
    isInfinity = false;
}

// default constructor for points at infinity
Curve::EC_point::EC_point() {
    x = y = NAN;
    isInfinity = true;
}

const double Curve::EC_point::getX() { return x; }

const double Curve::EC_point::getY() { return y; }

bool Curve::EC_point::isValid() { return (y*y + a1*x*y + a3*y) == (x*x*x + a2*x*x + a4*x + a6); }

void Curve::EC_point::print() { printf("(%.2f, %.2f)", x, y); }

void Curve::EC_point::println() { printf("(%.2f, %.2f)\n", x, y); }

// check for point equality
bool Curve::EC_point::operator==(Curve::EC_point a) {
    return abs(x - a.x) < DBL_EPSILON && abs(y - a.y) < DBL_EPSILON;
}

// check for point inequality
bool Curve::EC_point::operator!=(Curve::EC_point a) {
    return !(abs(x - a.x) < DBL_EPSILON && abs(y - a.y) < DBL_EPSILON);
}

// point addition
Curve::EC_point Curve::EC_point::operator+(Curve::EC_point a) {
    if (this->isInfinity && a.isInfinity) { // O+O
        return EC_point();
    }

    if (this->isInfinity && !a.isInfinity) { // O+P
        return a;
    }

    if (!this->isInfinity && a.isInfinity) { // P+O
        return *this;
    }

    if (*this == a) { // addition to itself
        return doublePoint();
    }
    
    const double lambda = (a.y - y)/(a.x - x);
    const double newX = lambda*lambda + a1*lambda - a2 - x - a.x;
    const double newY = -1*a1*newX - a3 - lambda*newX + lambda*x - y;
    return EC_point(newX, newY);
}

// point addition and assignment
Curve::EC_point Curve::EC_point::operator+=(Curve::EC_point a) {
    if (this->isInfinity && !a.isInfinity) { // O+P
        this->isInfinity = false;
        *this = a;
        return a;
    }

    *this = *this + a;
    return *this;
}

/**
 * @brief naive single-scalar multipliation (doesnt work)
 * 
 * @param n the scalar to multiply by
 * @return EC_point
 */
Curve::EC_point Curve::EC_point::operator*(unsigned n) {
    EC_point result{};
    for (size_t i = 0; i < n; i++) {
        result += *this; // breaks for n >= 6        
    }
    return result;
}

// point assignment
Curve::EC_point Curve::EC_point::operator=(Curve::EC_point point) {
    x = point.getX();
    y = point.getY();
    isInfinity = point.isInfinity;
    return *this;
}

/**
 * @brief double-and-add single-scalar multipliation
 * 
 * @param n the scalar to multiply by
 * @return EC_point
 */
Curve::EC_point Curve::EC_point::times(unsigned n) {
    if (n == 0) return EC_point(); // 0*P = O
    if (n == 1) return *this;
    if (n == 2) return doublePoint();

    EC_point result{};
    EC_point temp(x, y);

    while (n > 0) {
        unsigned long long b = n & 1;
        if (b == 1) {
            result += temp;
        }
        temp = temp.doublePoint();
        n >>= 1;
    }
    
    return result;
}

// point doubling
Curve::EC_point Curve::EC_point::doublePoint() {
    const double lambda = (3*x*x + 2*a2*x - a1*y + a4)/(2*y + a1*x + a3);
    const double x3 = lambda*lambda + lambda*a1 - a2 - 2*x;
    const double y3 = -1*a1*x3 - a3 - lambda*x3 + lambda*x - y;
    return EC_point(x3, y3);
}

// generates a point given some x
Curve::EC_point Curve::EC_point::generatePoint(double x) {
    double y = sqrt(x*x*x + 3);
    return EC_point(x, y);
}

void Curve::print() {
    printf("y^2 + %dxy + %dy = x^3 + %dx^2 + %dx + %d\n", a1, a3, a2, a4, a6);
}
