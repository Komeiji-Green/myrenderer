cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(RayTracing PathTracingTest.cpp Bounds3.hpp Vector.hpp core.hpp Triangle.hpp Scene.cpp
        Scene.hpp Ray.hpp MaterialFactory.hpp Material.hpp DiffuseMaterial.hpp ImportanceSampling.hpp Sampler.hpp
        Intersect.hpp Renderer.cpp Renderer.hpp PathTracingRenderer.hpp OBJ_Loader.hpp light.hpp)

add_executable(Phong ImSampleTest.cpp Bounds3.hpp Vector.hpp core.hpp Triangle.hpp Scene.cpp
        Scene.hpp Ray.hpp MaterialFactory.hpp Material.hpp DiffuseMaterial.hpp ImportanceSampling.hpp Sampler.hpp
        Intersect.hpp Renderer.cpp Renderer.hpp PathTracingRenderer.hpp OBJ_Loader.hpp light.hpp Phong.hpp Sphere.hpp ImSplRenderer.hpp)

target_link_libraries(RayTracing PRIVATE Threads::Threads)
target_link_libraries(Phong PRIVATE Threads::Threads)
