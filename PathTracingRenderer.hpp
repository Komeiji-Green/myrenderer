#ifndef PATHTRACING_H
#define PATHTRACING_H

#include "Renderer.hpp"
#include "Intersect.hpp"
#include "Scene.hpp"
#include "core.hpp"
#include "Sampler.hpp"

class PathTracingRenderer : public Renderer {
public:
    PathTracingRenderer(Sampler *sampler, int spp): sampler(sampler), spp(spp) {}

    Sampler *sampler;
    float RussianRoulette = 0.8;
    int spp;

    Vector3f radiance(const Scene& scene, const Ray& ray, int depth) const override
    {
        static const float l_eps = 1e-3;

        float invSpp = 1.f / spp;
        // if (depth >= 1) {
            // return Vector3f(0.0);
        // }

        Intersection u = scene.Intersect(ray);

        // std::cout << u.valid << std::endl;

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
            std::vector<Vector2f> seq = sampler->MultiSample(spp);
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
                            / (v.dist * v.dist) / pdf_light) * invSpp;
                    // std::cout << "emit = " << v.emit << std::endl;
                    // std::cout << "theta_u " << theta_u << std::endl;
                    // std::cout << "theta_v " << theta_v << std::endl;
                    // std::cout << "v.dist " << v.dist << std::endl;
                    // std::cout << "pdf_light = " << pdf_light << std::endl;
                    // std::cout << "eval = " << u.m->eval(wo, wi, u.normal) << std::endl;
                }
            }
        }

        // suppose that emit-material do not reflect light

        // reflect

        if (depth == 0) {
            std::vector<Vector2f> seq = sampler->MultiSample(spp);
            for (const Vector2f& seed : seq) {
                Vector3f wi = u.m->sample(wo, u.normal, seed);
                L_indir += (radiance(scene, Ray(u.coords, wi), depth + 1) * u.m->eval(wo, wi, u.normal) * dotProduct(wi, u.normal) 
                    / u.m->pdf(wo, wi, u.normal)) * invSpp;
            }
        } else {
            if (get_random_float() < RussianRoulette) {
                Vector3f wi = u.m->sample(wo, u.normal, sampler->Sample());
                L_indir = radiance(scene, Ray(u.coords, wi), depth + 1) * u.m->eval(wo, wi, u.normal) * dotProduct(wi, u.normal)
                    / u.m->pdf(wo, wi, u.normal) / RussianRoulette;
            // std::cout << "eval_refl = " << u.m->eval(wo, wi, u.normal) << std::endl;
            }
        }
        
        //  std::cout << "L_dir = " << L_dir << std::endl;
        //  std::cout << "L_indir = " << L_indir << std::endl;
        //  std::cout << "L_light = " << L_light << std::endl;

        return L_dir + L_indir + L_light;
    }
};

#endif
