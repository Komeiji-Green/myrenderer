#ifndef CORE_HPP
#define CORE_HPP

typedef float db;

#include "Vector.hpp"
#include <cassert>
#include <iostream>
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>
#include <string>
#include <array>
#include <vector>

inline constexpr float eps = 1e-5;
inline constexpr float InvPi = 1.0 / M_PI;

inline  bool solveQuadratic(const double &a, const double &b, const double &c, double &x0, double &x1)
{
    double discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = - 0.5 * b / a;
    else {
        double q = (b > 0) ?
                  -0.5 * (b + sqrt(discr)) :
                  -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);
    return true;
}

inline db get_random_float() {
    static std::mt19937 eng(time(NULL));
    std::uniform_real_distribution<db> dis(0.0, 1.0);
    return dis(eng);
}

template<typename T>
inline void rshuffle(std::vector<T> vec) {
    static std::mt19937 eng(time(NULL));
    std::shuffle(vec.begin(), vec.end(), eng);
}


inline float clamp(const float &lo, const float &hi, const float &v)
{ return std::max(lo, std::min(hi, v)); }

inline void UpdateProgress(float progress)
{
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
};

#endif