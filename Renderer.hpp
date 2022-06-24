#ifndef RENDERER_H
#define RENDERER_H

#include "Scene.hpp"
#include "Vector.hpp"
#include "Ray.hpp"
#include <vector>


class Renderer {
public:
    void Render(const Scene& scene) const;
    virtual Vector3f radiance(const Scene& scene, const Ray& ray, int depth) const = 0;
};

#endif