#ifndef BOUNDS_H
#define BOUNDS_H

#include "core.hpp"
#include "Ray.hpp"

class Bounds3 {
public:
    Bounds3() {}
    Bounds3(const Vector3f& a, const Vector3f& b): pMin(a), pMax(b) {}
    bool IntersectP(const Ray& ray) const
    {
        // invDir: ray direction(x,y,z), invDir=(1.0/x,1.0/y,1.0/z), use this because Multiply is faster that Division
        // dirIsNeg: ray direction(x,y,z), dirIsNeg=[int(x>0),int(y>0),int(z>0)], use this to simplify your logic

        double tmin = 0.0;
        double tmax = std::numeric_limits<double>::infinity();

        for (int i = 0; i < 3; i++) {
            if (ray.direction[i] == 0.0) {
                if (ray.origin[i] < pMin[i] || ray.origin[i] > pMax[i]) {
                    return false;
                }
            } else {
                double l = (pMin[i] - ray.origin[i]) * ray.direction_inv[i];
                double r = (pMax[i] - ray.origin[i]) * ray.direction_inv[i];

                if (ray.direction[i] <= 0.0)
                    std::swap(l, r);

                tmin = std::max(tmin, l);
                tmax = std::min(tmax, r);
            }
        }

        // std::cout << "ans: " << tmin << ' ' << tmax << std::endl;

        return tmin <= tmax;

    }
    
private:
    Vector3f pMin, pMax;
};

#endif