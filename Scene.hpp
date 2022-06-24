#ifndef SCENE_H
#define SCENE_H

#include "core.hpp"
#include "Intersect.hpp"
#include "Shape.hpp"
#include "Light.hpp"

class Scene {
public:
    Scene(int w, int h): width(w), height(h) {}
    Intersection Intersect(const Ray& ray) const;
    void SampleLight(Intersection& isect, db& pdf) const;
    void BuildBVH();
    void Add(Shape* shape);
    void SetLight(Light light) {
        this->light = light;
    }

    const int width = 1280, height = 960;
    const double fov = 40.0;
    Vector3f backgroundColor = Vector3f(0.235294, 0.67451, 0.843137);

    void show() {
        for (auto shape : shapes) {
            std::cout << shape->hasEmit() << std::endl;
            Intersection isect;
            float pdf;
            shape->Sample(isect, pdf);
            std::cout << isect.emit << std::endl;
        }
    }

    Light light;
    
private:
    std::vector<Shape*> shapes;
};

#endif