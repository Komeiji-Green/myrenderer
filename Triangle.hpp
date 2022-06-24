#pragma once

#include "Intersect.hpp"
#include "Material.hpp"
#include "OBJ_Loader.hpp"
#include "Shape.hpp"
#include "Triangle.hpp"
#include "core.hpp"
#include "Bounds3.hpp"

class Triangle : public Shape
{
public:
    Vector3f v0, v1, v2; // vertices A, B ,C , counter-clockwise order
    Vector3f e1, e2;     // 2 edges v1-v0, v2-v0;
    Vector3f normal;
    float area;
    Material* m;

    Bounds3 bounding_box;

    Triangle(Vector3f _v0, Vector3f _v1, Vector3f _v2, Material* _m)
        : v0(_v0), v1(_v1), v2(_v2), m(_m)
    {
        e1 = v1 - v0;
        e2 = v2 - v0;
        normal = normalize(crossProduct(e1, e2));
        area = crossProduct(e1, e2).norm()*0.5f;
        bounding_box = Bounds3(Vector3f::Min(Vector3f::Min(v0, v1), v2), Vector3f::Max(Vector3f::Max(v0, v1), v2));
    }

    Intersection Intersect(const Ray& ray) const override;
    Bounds3 getBounds() const override {
        return bounding_box;
    }

    void Sample(Intersection &pos, float &pdf) const override {
        float x = std::sqrt(get_random_float()), y = get_random_float();
        pos.coords = v0 * (1.0f - x) + v1 * (x * (1.0f - y)) + v2 * (x * y);
        pos.normal = this->normal;
        pos.m = m;
        pos.emit = m->getEmission();
        pdf = 1.0f / area;
    }

    float getArea() const override {
        return area;
    }
    bool hasEmit() const override {
        return m->hasEmit();
    }
};

class MeshTriangle : public Shape
{
public:
    MeshTriangle(const std::string& filename, Material *mt)
    {
        objl::Loader loader;
        loader.LoadFile(filename);
        area = 0;
        m = mt;
        assert(loader.LoadedMeshes.size() == 1);
        auto mesh = loader.LoadedMeshes[0];

        Vector3f min_vert = Vector3f{std::numeric_limits<float>::infinity(),
                                     std::numeric_limits<float>::infinity(),
                                     std::numeric_limits<float>::infinity()};
        Vector3f max_vert = Vector3f{-std::numeric_limits<float>::infinity(),
                                     -std::numeric_limits<float>::infinity(),
                                     -std::numeric_limits<float>::infinity()};
        for (int i = 0; i < mesh.Vertices.size(); i += 3) {
            std::array<Vector3f, 3> face_vertices;

            for (int j = 0; j < 3; j++) {
                auto vert = Vector3f(mesh.Vertices[i + j].Position.X,
                                     mesh.Vertices[i + j].Position.Y,
                                     mesh.Vertices[i + j].Position.Z);
                face_vertices[j] = vert;

                min_vert = Vector3f(std::min(min_vert.x, vert.x),
                                    std::min(min_vert.y, vert.y),
                                    std::min(min_vert.z, vert.z));
                max_vert = Vector3f(std::max(max_vert.x, vert.x),
                                    std::max(max_vert.y, vert.y),
                                    std::max(max_vert.z, vert.z));
            }

            triangles.emplace_back(face_vertices[0], face_vertices[1],
                                   face_vertices[2], mt);
            area += triangles.back().getArea();
        }

        bounding_box = Bounds3(min_vert, max_vert);
    }

    Bounds3 getBounds() const override { return bounding_box; }

    Intersection Intersect(const Ray& ray) const override
    {
        Intersection intersec;

        for (size_t i = 0; i < triangles.size(); i++) {
            intersec += triangles[i].Intersect(ray);
        }

        return intersec;
    }

    void Sample(Intersection &pos, float &pdf) const override;

    float getArea() const override {
        return area;
    }
    bool hasEmit() const override {
        return m->hasEmit();
    }

    Bounds3 bounding_box;
    uint32_t numTriangles;

    std::vector<Triangle> triangles;

    float area;

    Material* m;
};

inline Intersection Triangle::Intersect(const Ray& ray) const
{
    Intersection inter;

    if (dotProduct(ray.direction, normal) > 0)
        return inter;
    double u, v, t_tmp = 0;
    Vector3f pvec = crossProduct(ray.direction, e2);
    double det = dotProduct(e1, pvec);
    if (fabs(det) < eps)
        return inter;

    double det_inv = 1. / det;
    Vector3f tvec = ray.origin - v0;
    u = dotProduct(tvec, pvec) * det_inv;
    if (u < 0 || u > 1)
        return inter;
    Vector3f qvec = crossProduct(tvec, e1);
    v = dotProduct(ray.direction, qvec) * det_inv;
    if (v < 0 || u + v > 1)
        return inter;
    t_tmp = dotProduct(e2, qvec) * det_inv;

    if (t_tmp < 0.0) return inter;

    inter.valid = true;
    inter.normal = this->normal;
    inter.m = this->m;
    inter.emit = inter.m->getEmission();
    inter.dist = t_tmp;
    inter.coords = Vector3f(ray.origin + ray.direction * t_tmp);

    return inter;
}

void MeshTriangle::Sample(Intersection &pos, float &pdf) const {
    float bd = get_random_float() * area;
    float now = 0.;
    
    for (size_t i = 0; i < triangles.size(); i++) {
        now += triangles[i].getArea();
        if (now >= bd) {
            triangles[i].Sample(pos, pdf);
            pdf = 1.0f / area;
            break;
        }
    }
}
