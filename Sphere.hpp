#pragma once

#include "Shape.hpp"
#include "Vector.hpp"
#include "Bounds3.hpp"
#include "Material.hpp"

class Sphere : public Shape {
public:
    Vector3f center;
    float radius, radius2;
    Material *m;
    float area;
    Sphere(const Vector3f &c, const float &r, Material* mt) : center(c), radius(r), radius2(r * r), m(mt), area(4 * M_PI *r *r) {}

    Intersection Intersect(const Ray& ray) const override {
        Intersection result;
        result.valid = false;

        Bounds3 box = getBounds();
        if (!box.IntersectP(ray)) {
            return result;
        }

        Vector3f L = ray.origin - center;
        double a = dotProduct(ray.direction, ray.direction);
        double b = 2 * dotProduct(ray.direction, L);
        double c = dotProduct(L, L) - radius2;
        double t0, t1;
        if (!solveQuadratic(a, b, c, t0, t1)) return result;
        if (t0 < 1e-1) t0 = t1;
        if (t0 < 1e-1) return result;
        result.valid = true;

        result.coords = Vector3f(ray.origin + ray.direction * t0);
        result.normal = normalize(Vector3f(result.coords - center));
        result.m = this->m;
        result.dist = t0;

        return result;

    };

    Bounds3 getBounds() const override {
        return Bounds3(Vector3f(center.x-radius, center.y-radius, center.z-radius),
                       Vector3f(center.x+radius, center.y+radius, center.z+radius));
    }
    void Sample(Intersection &pos, float &pdf) const override {
        float theta = 2.0 * M_PI * get_random_float(), phi = M_PI * get_random_float();
        Vector3f dir(std::cos(phi), std::sin(phi)*std::cos(theta), std::sin(phi)*std::sin(theta));
        pos.coords = center + radius * dir;
        pos.normal = dir;
        pos.emit = m->getEmission();
        pdf = 1.0f / area;
    }
    float getArea() const override {
        return area;
    }
    bool hasEmit() const override {
        return m->hasEmit();
    }
};
