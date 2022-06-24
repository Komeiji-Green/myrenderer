//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::BuildBVH() {
}

void Scene::Add(Shape *v) {
    shapes.push_back(v);
}

Intersection Scene::Intersect(const Ray &ray) const
{  
    Intersection isect;
    for (size_t i = 0; i < shapes.size(); i++) {
        isect += shapes[i]->Intersect(ray);
        /*if (shapes[i]->getBounds().IntersectP(ray)) {
            isect += shapes[i]->Intersect(ray);
        }*/
    }
    return isect;
}


void Scene::SampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < shapes.size(); ++k) {
        if (shapes[k]->hasEmit()){
            emit_area_sum += shapes[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    float now_area = 0.;
    for (uint32_t k = 0; k < shapes.size(); ++k) {
        if (shapes[k]->hasEmit()){
            now_area += shapes[k]->getArea();
            if (p <= now_area){
                shapes[k]->Sample(pos, pdf);
                pdf = 1.0f / now_area;
                break;
            }
        }
    }
}