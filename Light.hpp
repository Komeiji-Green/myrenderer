#pragma once

#include "Shape.hpp"
#include "core.hpp"
#include "Intersect.hpp"

class Light { // square light
public:
    Light() {}
    Light(Vector3f a, Vector3f b, Vector3f c, Material *mt): mt(mt) {
        o = a;
        d1 = b - a;
        d2 = c - a;
        normal = normalize(crossProduct(d1, d2));
        area = crossProduct(d1, d2).norm();
        pdf = 1.f / area;
    }

    Vector3f o, d1, d2;
    Vector3f normal;
    Material *mt;
    float area;
    float pdf;

    Intersection sample(Vector2f uv, float& pdf) const {
        Intersection isect;
        isect.coords = o + (d1 * uv.x + d2 * uv.y);
        isect.emit = this->mt->getEmission();
        isect.m = this->mt;
        isect.normal = this->normal;
        pdf = this->pdf;
        return isect;
    }
};