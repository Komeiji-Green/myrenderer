#pragma once
#include "ImportanceSampling.hpp"
#include "DiffuseMaterial.hpp"
#include "Phong.hpp"

inline Material* MakeMaterial(Vector3f Kd, Vector3f emission, bool flag) {
    if (flag) {
        return new ImportanceMaterial(Kd, emission);
    } else {
        return new DiffuseMaterial(Kd, emission);
    }
}