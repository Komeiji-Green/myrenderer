#ifndef RAY_HPP
#define RAY_HPP

#include "Vector.hpp"

class Ray {
public:
    Ray(const Vector3f& o, const Vector3f& d): origin(o), direction(d) {
        direction_inv = Vector3f(1.0f / d.x, 1.0f / d.y, 1.0f / d.z);
    }

    Vector3f origin, direction, direction_inv;
};

#endif