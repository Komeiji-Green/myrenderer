#pragma once

#include "core.hpp"
#include "Material.hpp"
#include "Sampler.hpp"

class Phong : public Material {
public:
    Phong(Vector3f Kd, Vector3f Ks, int p): Material(Kd, Vector3f(0.0f)), Kd(Kd), Ks(Ks), p(p) {}

    Vector3f sample(const Vector3f& wo, const Vector3f& N, const Vector2f& u) const override {
        return sampleDiff(wo, N, u);
    }

    db pdf(const Vector3f &wo, const Vector3f& wi, const Vector3f& N) const override {
        return pdfDiff(wo, wi, N);
    }

    Vector3f sampleSpec(const Vector3f& wo, const Vector3f& N, const Vector2f& u) const {
        float s1 = u.x, s2 = u.y;
        float z = std::pow(s1, 1.f / (p + 1));
        float r = std::sqrt(std::max(0.f, 1.f - z * z));
        Vector3f h = Material::toWorld(Vector3f(r * std::cos(2.f * M_PI * s2), r * std::sin(2.f * M_PI * s2), z), N);
        return h * (2.f * dotProduct(wo, h)) - wo;
    };

    float pdfSpec(const Vector3f& wo, const Vector3f& wi, const Vector3f& N) const {
        if (dotProduct(wi, N) > 0.f) {
            float cosTheta = dotProduct(normalize(wo + wi), N);
            return ((1.f + p) / (2.f * M_PI)) * std::pow(cosTheta, p);
        } else {
            return 0.0f;
        }
    }

    Vector3f sampleDiff(const Vector3f& wo, const Vector3f& N, const Vector2f& u) const {
        Vector2f xy = ConcentricSampleDisk(u);
        float z = std::sqrt(1. - xy.x * xy.x - xy.y * xy.y);

        return Material::toWorld(Vector3f(xy.x, xy.y, z), N);
    };   

    float pdfDiff(const Vector3f& wo, const Vector3f& wi, const Vector3f& N) const {
        float cosTheta = dotProduct(wi, N);
        if (cosTheta > 0.0f) {
            return cosTheta * InvPi;
        } else {
            return 0.0f;
        }
    }

    Vector3f eval(const Vector3f& wo, const Vector3f& wi, const Vector3f& N) const override {
        if (dotProduct(wo, N) > 0.f && dotProduct(wi, N) > 0.f) {
            Vector3f h = normalize(wo + wi);
            return Kd / M_PI + Ks * std::max(0.0, std::pow(dotProduct(h, N), p));
        } else {
            return Vector3f(0.f);
        } 
    };

private:
    int p;
    Vector3f Kd;
    Vector3f Ks;
};