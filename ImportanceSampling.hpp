#pragma once
#ifndef IM_MATERIAL_H
#define IM_MATERIAL_H

#include "core.hpp"
#include "Material.hpp"
#include "Sampler.hpp"

class ImportanceMaterial : public Material {
public:
    ImportanceMaterial(Vector3f Kd, Vector3f e): Material(Kd, e) {}
    Vector3f sample(const Vector3f& wo, const Vector3f& N, const Vector2f& u) const override;
    db pdf(const Vector3f &wo, const Vector3f& wi, const Vector3f& N) const override;
    Vector3f eval(const Vector3f& wo, const Vector3f& wi, const Vector3f& N) const override;
};

Vector3f ImportanceMaterial::sample(const Vector3f &wo, const Vector3f &N, const Vector2f& u) const {
    // cosine importance sample on the hemisphere
    Vector2f xy = ConcentricSampleDisk(u);
    float z = std::sqrt(1. - xy.x * xy.x - xy.y * xy.y);

    return Material::toWorld(Vector3f(xy.x, xy.y, z), N);
}

db ImportanceMaterial::pdf(const Vector3f &wo, const Vector3f &wi, const Vector3f &N) const {
    // cosine sample probability 1 / (2 * PI)
    float cosTheta = dotProduct(wi, N);
    if (cosTheta > 0.0f) {
        return cosTheta * InvPi;
    } else {
        return 0.0f;
    }
}

Vector3f ImportanceMaterial::eval(const Vector3f &wo, const Vector3f &wi, const Vector3f &N) const {
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