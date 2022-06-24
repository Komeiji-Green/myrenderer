#include "Renderer.hpp"
#include "Scene.hpp"
#include "Triangle.hpp"
#include "core.hpp"
#include "DiffuseMaterial.hpp"
#include "PathTracingRenderer.hpp"
#include "Sampler.hpp"
#include <chrono>
#include <cstdlib>
#include <cstring>
#include "MaterialFactory.hpp"

// In the main function of the program, we create the scene (create objects and
// lights) as well as set the options for the render (image width and height,
// maximum recursion depth, field-of-view, etc.). We then call the render
// function().
int main(int argc, char** argv)
{
    bool importance = false;
    bool lowdep = false;
    
    int spnum = 1;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-im") == 0) {
                importance = true;
            }
            if (strcmp(argv[i], "-lo") == 0) {
                lowdep = true;
            }
        } else {
            spnum = atoi(argv[i]);
        }
    }

    // Change the definition here to change resolution
    Scene scene(784, 784);

    Vector3f red_kd(0.63f, 0.065f, 0.05f);
    Vector3f green_kd(0.14f, 0.45f, 0.091f);
    Vector3f white_kd(0.725f, 0.71f, 0.68f);
    Vector3f light_kd(0.65f);

    Material* red = MakeMaterial(red_kd, Vector3f(0.0f), importance);
    Material* green = MakeMaterial(green_kd, Vector3f(0.0f), importance);
    Material* white = MakeMaterial(white_kd, Vector3f(0.0f), importance);
    Material* light = MakeMaterial(light_kd, (8.0f * Vector3f(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * Vector3f(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f *Vector3f(0.737f+0.642f,0.737f+0.159f,0.737f)), importance);

    MeshTriangle floor("../models/cornellbox/floor.obj", white);
    MeshTriangle shortbox("../models/cornellbox/shortbox.obj", white);
    MeshTriangle tallbox("../models/cornellbox/tallbox.obj", white);
    MeshTriangle left("../models/cornellbox/left.obj", red);
    MeshTriangle right("../models/cornellbox/right.obj", green);
    MeshTriangle light_("../models/cornellbox/light.obj", light);

    Light squarelight(Vector3f(213.0, 548.7, 227.0), Vector3f(343.0, 548.7, 227.0), Vector3f(213.0, 548.7, 332.0), light);

    scene.Add(&floor);
    scene.Add(&shortbox);
    scene.Add(&tallbox);
    scene.Add(&left);
    scene.Add(&right);
    scene.Add(&light_);
    
    scene.SetLight(squarelight);

    scene.show();

    Sampler* sampler = nullptr;
    if (lowdep) {
        sampler = new Jitter();
    } else {
        sampler = new White();
    }

    Renderer *r = new PathTracingRenderer(sampler, spnum);

    auto start = std::chrono::system_clock::now();
    r->Render(scene);
    auto stop = std::chrono::system_clock::now();

    std::cout << "Render complete: \n";
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " minutes\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n";

    return 0;
}