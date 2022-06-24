#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "Intersect.hpp"
#include "Ray.hpp"
#include "Bounds3.hpp"

class Shape {
public:
    virtual Bounds3 getBounds() const = 0;
    virtual float getArea() const = 0;
    virtual void Sample(Intersection &pos, float &pdf) const = 0;
    virtual bool hasEmit() const = 0;

    virtual Intersection Intersect(const Ray& ray) const = 0;
};

#endif
