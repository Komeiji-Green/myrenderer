#pragma once
#ifndef DIFF_MATERIAL_H
#define DIFF_MATERIAL_H

#include "core.hpp"
#include "Material.hpp"

class DiffuseMaterial : public Material {
public:
    DiffuseMaterial(Vector3f Kd, Vector3f e): Material(Kd, e) {}
    Vector3f sample(const Vector3f& wo, const Vector3f& N, const Vector2f& u) const override;
    db pdf(const Vector3f &wo, const Vector3f& wi, const Vector3f& N) const override;
    Vector3f eval(const Vector3f& wo, const Vector3f& wi, const Vector3f& N) const override;
};

Vector3f DiffuseMaterial::sample(const Vector3f &wo, const Vector3f &N, const Vector2f& u) const {
    // uniform sample on the hemisphere
    float z = std::fabs(1.0f - 2.0f * u.x);
    float r = std::sqrt(1.0f - z * z), phi = 2 * M_PI * u.y;
    Vector3f localRay(r*std::cos(phi), r*std::sin(phi), z);
    return Material::toWorld(localRay, N);
}

db DiffuseMaterial::pdf(const Vector3f &wo, const Vector3f &wi, const Vector3f &N) const {
    // uniform sample probability 1 / (2 * PI)
    if (dotProduct(wi, N) > 0.0f)
        return 0.5f / M_PI;
    else
        return 0.0f;
}

Vector3f DiffuseMaterial::eval(const Vector3f &wo, const Vector3f &wi, const Vector3f &N) const {
    // BSDF
    float cos_o = dotProduct(N, wo);
    float cos_i = dotProduct(N, wi);
    if (cos_o > 0.0f && cos_i > 0.0f) {
        Vector3f diffuse = Kd / M_PI;
        return diffuse;
    } else {
        return Vector3f(0.0f);
    }
}

#endif