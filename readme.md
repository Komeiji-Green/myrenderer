## 平台

macOS 或 ubuntu

## 编译命令
```
mkdir build
cd build
cmake ..
make
```

## 程序如何运行

./Phong [每个点进行漫反射采样的次数] [进行镜面反射采样的次数] [采样光源的次数] [非 Phong 材质采样半球面的次数] [Phong 模型表面光滑系数 P] [多重重要性采样组合系数 k] [Phong 模型 specular reflaction 光强 Ks]

./RayTracing -im -lo [采样次数]
用 -im 开启重要性采样，用 -lo 使用低差异化序列，注意当开启 -lo 选项时，采样次数必须为完全平方数

所有采样次数建议设置在 64 以内

示范：
- ./Phong 5 5 5 4 5 2 0.15 (生成 test1.ppm)
- ./RayTracing -im -lo 9 (生成 test2.ppm)

生成结果为 build 文件夹中的 binary.ppm


## 基本模块：
0. 向量 Vector
1. 形状 Shape
    球面 Sphere
    三角形 Triangle
2. 光线 Ray
3. 光源 Light
4. 材质 Material
    漫反射
    Phong 模型
5. 采样器 Sampler
    白噪声
    低差异化序列
6. 光线追踪求解器 Renderer



