#ifndef Material_HPP
#define Material_HPP

#include "core.hpp"

class Material {
public:
    Material(Vector3f Kd, Vector3f emission): Kd(Kd), _emission(emission) {
        _hasEmit = emission.norm() > eps;
        if (_hasEmit) {
            // std::cout << "YES" << std::endl;
        }
    }

    virtual Vector3f sample(const Vector3f& wo, const Vector3f& N, const Vector2f& u) const = 0;
    virtual db pdf(const Vector3f &wo, const Vector3f& wi, const Vector3f& N) const = 0;
    virtual Vector3f eval(const Vector3f& wo, const Vector3f& wi, const Vector3f& N) const = 0;


    // Light hardcode in Material

    bool hasEmit() const {
        return _hasEmit;
    }
    Vector3f getEmission() const {
        return _emission;
    }


    // static method

    // required: N is normalized
    static Vector3f toWorld(const Vector3f& a, const Vector3f& N) {
        Vector3f A, B;

        if (fabs(N.x) > fabs(N.y)) {
            double invLen = sqrt(N.x * N.x + N.z * N.z);
            A = Vector3f(-N.z / invLen, 0., N.x / invLen);
        } else {
            double invLen = sqrt(N.y * N.y + N.z * N.z);
            A = Vector3f(0., -N.z / invLen, N.y / invLen);
        }

        B = crossProduct(A, N);

        return A * a.x + B * a.y + N * a.z;
    }
protected:
    bool _hasEmit;
    Vector3f _emission;
    Vector3f Kd;
};


#endif
