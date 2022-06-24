#include <fstream>
#include <thread>
#include <future>
#include <numeric>
#include "Scene.hpp"
#include "Renderer.hpp"
#include <iostream>

inline float deg2rad(const float& deg) { return deg * M_PI / 180.0; }

const float EPSILON = 0.00001;
const int MAXSPP = 1;
const int thread_num = 4;

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene& scene) const 
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = tan(deg2rad(scene.fov * 0.5));
    float imageAspectRatio = scene.width / (float)scene.height;
    Vector3f eye_pos(278, 273, -800);
    int m = 0;

    int spp = MAXSPP; // sample per pixel

    std::cout << "SPP: " << spp << "\n";
    for (uint32_t j_big = 0; j_big < scene.height; j_big += thread_num) {
        auto work = [&](int j) -> std::vector<Vector3f> {
            std::vector<Vector3f> col(scene.width);
            for (uint32_t i = 0; i < scene.width; ++i) {
                float x = (2 * (i + 0.5) / (float)scene.width - 1) *
                        imageAspectRatio * scale;
                float y = (1 - 2 * (j + 0.5) / (float)scene.height) * scale;

                Vector3f dir = normalize(Vector3f(-x, y, 1));

                for (int k = 0; k < spp; k++) {
                    col[i] += this->radiance(scene, Ray(eye_pos, dir), 0) / spp;
                }   
            }
            return col;
        };
        std::vector<std::future<std::vector<Vector3f>>> cols;
        for (uint32_t j = j_big; j < j_big + thread_num && j < scene.height; j++) {
            cols.push_back(std::async(work, j));
        }
        for (uint32_t j = j_big; j < j_big + thread_num && j < scene.height; j++) {
            std::vector<Vector3f> col = cols[j - j_big].get();
            for (uint32_t i = 0; i < scene.width; i++) {
                framebuffer[m++] = col[i];
            }
            UpdateProgress(j / (float)scene.height);
        }
    }
    UpdateProgress(1.f);

    // save framebuffer to file
    FILE* fp = fopen("binary.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
    for (auto i = 0; i < scene.height * scene.width; ++i) {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].x), 0.6f));
        color[1] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].y), 0.6f));
        color[2] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].z), 0.6f));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);    
}
