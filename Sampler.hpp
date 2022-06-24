#pragma once
#include "core.hpp"

inline Vector2f ConcentricSampleDisk(const Vector2f& u) {
    Vector2f uOff = u * 2.f + Vector2f(-1, -1);

    if (uOff.x == 0 && uOff.y == 0) {
        return Vector2f(0, 0);
    } 
    
    float theta, r;
    if (std::abs(uOff.x) > std::abs(uOff.y)) {
        r = uOff.x;
        theta = M_PI * 0.25 * (uOff.y / uOff.x);
    } else {
        r = uOff.y;
        theta = M_PI * (0.5 - 0.25 * (uOff.x / uOff.y));
    }

    return Vector2f(r * std::cos(theta), r * std::sin(theta)); 
}

class Sampler {
public:
    virtual std::vector<Vector2f> MultiSample(int num) const = 0;
    virtual Vector2f Sample() const = 0;
};

class White : public Sampler {
public:
    std::vector<Vector2f> MultiSample(int num) const override {
        std::vector<Vector2f> vec(num);
        for (int i = 0; i < num; i++) {
            vec[i] = White::Sample();
        }
        return vec;
    }

    Vector2f Sample() const override {
        return Vector2f(get_random_float(), get_random_float());
    }
};

class Jitter : public Sampler {
public: 
    std::vector<Vector2f> MultiSample(int num) const override {
        int sq = sqrt(num);
        assert(sq * sq == num);

        std::vector<Vector2f> vec(num);

        float scale = 1.f / sq;
        for (int i = 0; i < sq; i++) {
            for (int j = 0; j < sq; j++) {
                vec[i * sq + j] = Vector2f(i + get_random_float(), j + get_random_float()) * scale;
            }
        }

        rshuffle(vec);
        return vec;
    }

    Vector2f Sample() const override {
        return Vector2f(get_random_float(), get_random_float());
    }
};
