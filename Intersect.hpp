#ifndef INTERSECT_HPP
#define INTERSECT_HPP

#include "Vector.hpp"
#include "Material.hpp"
#include "core.hpp"

class Intersection {
public:
    Vector3f normal; // norm vector
    Material *m;    // material
    db dist;    // distance
    bool valid;  // valid
    Vector3f coords;
    Vector3f emit;

    Intersection(): valid(false) {}

    /**
     * left Intersection can replace right,
     * in another word, left Intersection has smaller distance than right.
     */ 
    bool operator< (const Intersection& o) const {
        if (!valid)
            return false;
        else {
            if (!o.valid)
                return true;
            else {
                return dist < o.dist;
            }
        }
    }

    Intersection& operator+= (const Intersection& o) {
        if (o < (*this))
            (*this) = o;
        return (*this);
    };
};

#endif