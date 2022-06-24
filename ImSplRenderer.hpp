#pragma once

#include "Renderer.hpp"
#include "Intersect.hpp"
#include "Scene.hpp"
#include "core.hpp"
#include "Sampler.hpp"
#include "Phong.hpp"

inline float combineWeight(int nf, float pf, int ng, float pg, int k) {
    float x = std::pow(nf * pf, k);
    float y = std::pow(ng * pg, k);
    return x / (x + y);
}

class ImportanceSamplingRenderer : public Renderer {
public:
    ImportanceSamplingRenderer(Sampler *sampler, int nlight, int ndiff, int nspec, int spp, int combineK = 1): sampler(sampler), nlight(nlight), ndiff(ndiff), nspec(nspec), spp(spp), combineK(combineK) {}

    Sampler *sampler;
    float RussianRoulette = 0.8;
    int nlight, ndiff, nspec, spp, combineK;

    Vector3f radiance(const Scene& scene, const Ray& ray, int depth) const override
    {
        static const float l_eps = 1e-4;

        float invSpp = 1.f / spp;

        Intersection u = scene.Intersect(ray);

        if (!u.valid) {
            // printf("floor = %d\n", depth);
            return Vector3f(0.f);
        }

        Vector3f L_dir(0.f), L_indir(0.f), L_light(0.f);
        Vector3f wo = -ray.direction;

        if (depth == 0) {
            if (u.m->hasEmit()) {
                L_light = u.emit;
                // std::cout << "emit = " << u.emit << std::endl;
            }
        }

        { // Light
            std::vector<Vector2f> seq = sampler->MultiSample(nlight);
            float invNlight = 1.f / nlight;
            for (const Vector2f& seed : seq) {
                float pdf_light;
                Intersection v = scene.light.sample(seed, pdf_light);

                v.dist = (v.coords - u.coords).norm();
                // std::cout << "light emit = " << v.emit << std::endl;
                Vector3f wi = normalize(v.coords - u.coords);

                float theta_u = dotProduct(wi, u.normal);
                float theta_v = dotProduct(-wi, v.normal);

                Intersection tmp = scene.Intersect(Ray(u.coords, wi));

                if (tmp.valid && fabs(tmp.dist - v.dist) < l_eps) {
                    L_dir += (v.emit * theta_u * theta_v * u.m->eval(wo, wi, u.normal)
                            / (v.dist * v.dist) / pdf_light) * invNlight;
                }
            }
        }

        // reflect

        if (depth == 0) {
            Phong *mt = dynamic_cast<Phong*>(u.m);
            if (mt) {
                std::vector<Vector2f> sd = sampler->MultiSample(ndiff);
                std::vector<Vector2f> ss = sampler->MultiSample(nspec);
                
                for (const Vector2f& seed : sd) {
                    float invNdiff = 1.f / ndiff;
                    Vector3f wi = mt->sampleDiff(wo, u.normal, seed);
                    float pdfDiff = mt->pdfDiff(wo, wi, u.normal);
                    if (pdfDiff > 0.f) {
                        float pdfSpec = mt->pdfSpec(wo, wi, u.normal);
                        float w = combineWeight(ndiff, pdfDiff, nspec, pdfSpec, combineK);
                        L_indir += ((radiance(scene, Ray(u.coords, wi), depth + 1) * mt->eval(wo, wi, u.normal) * dotProduct(wi, u.normal) * w) / pdfDiff) * invNdiff; 
                    }
                }

                for (const Vector2f& seed : ss) {
                    float invNspec = 1.f / nspec;
                    Vector3f wi = mt->sampleSpec(wo, u.normal, seed);
                    float pdfSpec = mt->pdfSpec(wo, wi, u.normal);
                    if (pdfSpec > 0.f) {
                        float pdfDiff = mt->pdfDiff(wo, wi, u.normal);
                        float w = combineWeight(nspec, pdfSpec, ndiff, pdfDiff, combineK);
                        L_indir += ((radiance(scene, Ray(u.coords, wi), depth + 1) * mt->eval(wo, wi, u.normal) * dotProduct(wi, u.normal) * w) / pdfSpec) * invNspec;
                    }
                }
            } else {
                std::vector<Vector2f> seq = sampler->MultiSample(spp);
                for (const Vector2f& seed : seq) {
                    Vector3f wi = u.m->sample(wo, u.normal, seed);
                    float pdf = u.m->pdf(wo, wi, u.normal);
                    if (pdf > 0.f) {
                        L_indir += (radiance(scene, Ray(u.coords, wi), depth + 1) * u.m->eval(wo, wi, u.normal) * dotProduct(wi, u.normal) 
                            / pdf) * invSpp;
                    }
                }
            }
        } else {
            if (get_random_float() < RussianRoulette) {
                Vector3f wi = u.m->sample(wo, u.normal, sampler->Sample());
                float pdf = u.m->pdf(wo, wi, u.normal);
                if (pdf > 0.f) {
                    L_indir = radiance(scene, Ray(u.coords, wi), depth + 1) * u.m->eval(wo, wi, u.normal) * dotProduct(wi, u.normal)
                        / pdf / RussianRoulette;
                }
            // std::cout << "eval_refl = " << u.m->eval(wo, wi, u.normal) << std::endl;
            }
        }
        
        //  std::cout << "L_dir = " << L_dir << std::endl;
        //  std::cout << "L_indir = " << L_indir << std::endl;
        //  std::cout << "L_light = " << L_light << std::endl;

        return L_dir + L_indir + L_light;
    }
};
