#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>
#include <iostream>
#include "render.hpp"
#include "shaderManager.hpp"
#include "inputs.hpp"

extern GLFWwindow *window;
using glm::fvec3;
using glm::fvec4;
using glm::dvec3;
using glm::dvec4;
using glm::dmat4;

dmat4 player = dmat4(1);
double deltaTime = 0;

dmat4 movMat() {
    dvec3 ax;
    double a = getMov(ax) * deltaTime;
    if (a == 0)
        return dmat4(1);
    double c = cos(a);
    double v = 1 - c;
    double s = sin(a);
    dmat4 m;
    m[0] = dvec4(c, ax * s);
    m[1] = dvec4(-ax.x * s, 1 - ax.x * ax.x * v, -ax.x * ax.y * v, -ax.x * ax.z * v);
    m[2] = dvec4(-ax.y * s, -ax.x * ax.y * v, 1 - ax.y * ax.y * v, -ax.y * ax.z * v);
    m[3] = dvec4(-ax.z * s, -ax.x * ax.z * v, -ax.y * ax.z * v, 1 - ax.z * ax.z * v);
    return player * m * transpose(player);
}

dmat4 rotMat() {
    dvec3 ax;
    double a = getRot(ax) * deltaTime;
    double c = cos(a);
    double v = 1 - c;
    double s = sin(a);
    double x = ax.x, y = ax.y, z = ax.z;
    dmat4 m;
    m[0] = dvec4(1, 0, 0, 0);
    m[1] = dvec4(0, c + v * x * x, v * y * x + s * z, c * z * x - s * y);
    m[2] = dvec4(0, v * x * y - s * z, c + v * y * y, v * z * y + s * x);
    m[3] = dvec4(0, v * x * z + s * y, v * y * z - s * x, c + v * z * z);
    return player * m * transpose(player);
}

void repair(dmat4 &m) {
    m[0] = normalize(m[0]);
    m[1] = normalize(m[1] - m[0] * dot(m[0], m[1]));
    m[2] = normalize(m[2] - m[0] * dot(m[0], m[2]) - m[1] * dot(m[1], m[2]));
    m[3] = normalize(m[3] - m[0] * dot(m[0], m[3]) - m[1] * dot(m[1], m[3]) - m[2] * dot(m[2], m[3]));
}

void init() {
    renderLoad();   // this must be the first, I think
    glfwSetKeyCallback(window, keyCallback);

    // Face f1 = Face(fvec3(1, 0, 0), 4, dvec4(0, 0, -1, 1)  * .5, dvec4(1, -1, -1, 1) * .5, dvec4(1, 1, -1, 1) * .5, rad);
    double rad = .2;
    const int n = 5;
    fvec4 arr[n];
    double phi = 2 * std::numbers::pi / n;
    for (int i = 0; i < n; ++i) {
//        0., np.sin(rad) * np.cos(phi * i), np.sin(rad) * np.sin(phi * i), np.cos(rad)
        arr[i] = fvec4(0, sin(rad) * cos(phi * i), sin(rad) * sin(phi * i), cos(rad));
//        arr[i] = fvec4(cos(rad) * cos(phi * i), cos(rad) * sin(phi * i), sin(rad), 0);
    }
    vector<fvec4> vertexes{arr[0], arr[1], arr[2], arr[3], arr[4]};
    auto f1 = Face(fvec3(0, 1, 0), vertexes);
    regFace(f1);
}

void loop() {
    auto begin = std::chrono::high_resolution_clock::now();
    player = rotMat() * player;
    repair(player);
    player = movMat() * player;
    repair(player);
    setPlayer(player);
    renderLoop();   // this must be the last, I think
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration<double>(end - begin);
    deltaTime = std::chrono::duration<double>(elapsed_ms).count();
    // std::cout << "fps: " << 1/deltaTime << '\n';
}

int main() {
    init();
    while (!exitCondition())
        loop();
    glfwTerminate();
    return 0;
}
