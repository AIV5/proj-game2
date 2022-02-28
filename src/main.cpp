#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
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
using glm::linearRand;

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

static void mk_face(double rad, int n, fvec3 color) {
    vector<fvec4> vertexes(n);
    double phi = 2 * std::numbers::pi / n;
    for (int i = 0; i < n; ++i) {
        vertexes[i] = fvec4(0, sin(rad) * cos(phi * i), sin(rad) * sin(phi * i), cos(rad));
    }
    auto f1 = Face(color, vertexes);
    regFace(f1);
}

static void mk_334() {
    vector<fvec4> vertexes(3);

    vertexes = {fvec4(0, 1, 0, 0), fvec4(0, 0, 1, 0), fvec4(0, 0, 0, 1)};
    auto f = Face(fvec3(1, 0, 0), vertexes);
    regFace(f);
    vertexes = {fvec4(1, 0, 0, 0), fvec4(0, 0, 1, 0), fvec4(0, 0, 0, 1)};
    f = Face(fvec3(0, 1, 0), vertexes);
    regFace(f);

    vertexes = {fvec4(1, 0, 0, 0), fvec4(0, 1, 0, 0), fvec4(0, 0, 0, 1)};
    f = Face(fvec3(0, 0, 1), vertexes);
    regFace(f);

    vertexes = {fvec4(1, 0, 0, 0), fvec4(0, 1, 0, 0), fvec4(0, 0, 1, 0)};
    f = Face(fvec3(.5, 0, .5), vertexes);
    regFace(f);

    Sphere s = Sphere(fvec3(1, 1, 1), fvec4(.5, .5, .5, .5), 0.03);
    regSphere(s);
    s = Sphere(fvec3(.5, 0, 0), fvec4(.5, -.5, .5, .5), 0.03);
    regSphere(s);
    s = Sphere(fvec3(0, .5, 0), fvec4(.5, .5, -.5, .5), 0.03);
    regSphere(s);
    s = Sphere(fvec3(0, 0, .5), fvec4(.5, .5, .5, -.5), 0.03);
    regSphere(s);

    s = Sphere(fvec3(0, 0, 0), fvec4(.5, .5, -.5, -.5), 0.03);
    regSphere(s);
    s = Sphere(fvec3(.5, .5, 0), fvec4(.5, -.5, .5, -.5), 0.03);
    regSphere(s);
    s = Sphere(fvec3(0, .5, .5), fvec4(.5, -.5, -.5, .5), 0.03);
    regSphere(s);
    s = Sphere(fvec3(.5, 0, .5), fvec4(.5, -.5, -.5, -.5), 0.03);
    regSphere(s);
}

static void mk_433() {
    vector<fvec4> vertexes(4);
    double a = .5;

    // 0+
    vertexes = {fvec4(a, a, a, a), fvec4(a, a, a, -a), fvec4(a, a, -a, -a), fvec4(a, a, -a, a)};
    auto f = Face(fvec3(1, 0, 0), vertexes);
    regFace(f);

    vertexes = {fvec4(a, a, a, a), fvec4(a, a, a, -a), fvec4(a, -a, a, -a), fvec4(a, -a, a, a)};
    f = Face(fvec3(0, 1, 0), vertexes);
    regFace(f);

    vertexes = {fvec4(a, a, a, a), fvec4(a, a, -a, a), fvec4(a, -a, -a, a), fvec4(a, -a, a, a)};
    f = Face(fvec3(0, 0, 1), vertexes);
    regFace(f);

    // 0-
    vertexes = {fvec4(-a, a, a, a), fvec4(-a, a, a, -a), fvec4(-a, a, -a, -a), fvec4(-a, a, -a, a)};
    f = Face(fvec3(0, 1, 1), vertexes);
    regFace(f);

    vertexes = {fvec4(-a, a, a, a), fvec4(-a, a, a, -a), fvec4(-a, -a, a, -a), fvec4(-a, -a, a, a)};
    f = Face(fvec3(1, 0, 1), vertexes);
    regFace(f);

    vertexes = {fvec4(-a, a, a, a), fvec4(-a, a, -a, a), fvec4(-a, -a, -a, a), fvec4(-a, -a, a, a)};
    f = Face(fvec3(1, 1, 0), vertexes);
    regFace(f);

    Sphere s(fvec3(1, 1, 1), fvec4(1, 0, 0, 0), 0.03);
    regSphere(s);
//
//    // 1+
//    vertexes = {fvec4(a, a, a, a), fvec4(a, a, a, -a), fvec4(-a, a, a, -a), fvec4(-a, a, a, a)};
//    f = Face(fvec3(.5, .6, .1), vertexes);
//    regFace(f);
//
//    vertexes = {fvec4(a, a, a, a), fvec4(a, a, -a, a), fvec4(-a, a, -a, a), fvec4(-a, a, a, a)};
//    f = Face(fvec3(.5, .1, .6), vertexes);
//    regFace(f);
//
//    // 1-
//    vertexes = {fvec4(a, -a, a, a), fvec4(a, -a, a, -a), fvec4(-a, -a, a, -a), fvec4(-a, -a, a, a)};
//    f = Face(fvec3(.6, .5, .1), vertexes);
//    regFace(f);
//
//    vertexes = {fvec4(a, -a, a, a), fvec4(a, -a, -a, a), fvec4(-a, -a, -a, a), fvec4(-a, -a, a, a)};
//    f = Face(fvec3(.1, .5, .6), vertexes);
//    regFace(f);
//
//    s = Sphere(fvec3(.5, .3, .3), fvec4(0, 1, 0, 0), 0.03);
//    regSphere(s);
//
//    // 2+
//    vertexes = {fvec4(a, a, a, a), fvec4(a, -a, a, a), fvec4(-a, -a, a, a), fvec4(-a, a, a, a)};
//    f = Face(fvec3(.6, .1, .5), vertexes);
//    regFace(f);
//
//    // 2-
//    vertexes = {fvec4(a, a, -a, a), fvec4(a, -a, -a, a), fvec4(-a, -a, -a, a), fvec4(-a, a, -a, a)};
//    f = Face(fvec3(.6, .1, .5), vertexes);
//    regFace(f);
//
//    s = Sphere(fvec3(.3, .5, .3), fvec4(0, 0, 1, 0), 0.03);
//    regSphere(s);
//
//    s = Sphere(fvec3(.3, .3, .5), fvec4(0, 0, 0, 1), 0.03);
//    regSphere(s);
}

static void mk_343() {
    vector<fvec4> vertexes(4);
    double a = sqrt(.5);

    vertexes = {fvec4(0, 0, a, a), fvec4(0, a, 0, a), fvec4(0, a, a, 0)};
    auto f = Face(linearRand(fvec3(0), fvec3(1)), vertexes);
    regFace(f);

    vertexes = {fvec4(0, 0, a, a), fvec4(a, 0, 0, a), fvec4(a, 0, a, 0)};
    f = Face(linearRand(fvec3(0), fvec3(1)), vertexes);
    regFace(f);

    vertexes = {fvec4(0, a, 0, a), fvec4(a, 0, 0, a), fvec4(a, a, 0, 0)};
    f = Face(linearRand(fvec3(0), fvec3(1)), vertexes);
    regFace(f);

    vertexes = {fvec4(0, a, a, 0), fvec4(a, 0, a, 0), fvec4(a, a, 0, 0)};
    f = Face(linearRand(fvec3(0), fvec3(1)), vertexes);
    regFace(f);
//
    vertexes = {fvec4(a, a, 0, 0), fvec4(a, 0, a, 0), fvec4(a, 0, 0, a)};
    f = Face(linearRand(fvec3(0), fvec3(1)), vertexes);
    regFace(f);

    vertexes = {fvec4(a, a, 0, 0), fvec4(0, a, a, 0), fvec4(0, a, 0, a)};
    f = Face(linearRand(fvec3(0), fvec3(1)), vertexes);
    regFace(f);

    vertexes = {fvec4(a, 0, a, 0), fvec4(0, a, a, 0), fvec4(0, 0, a, a)};
    f = Face(linearRand(fvec3(0), fvec3(1)), vertexes);
    regFace(f);

    vertexes = {fvec4(a, 0, 0, a), fvec4(0, a, 0, a), fvec4(0, 0, a, a)};
    f = Face(linearRand(fvec3(0), fvec3(1)), vertexes);
    regFace(f);

    Sphere s(linearRand(fvec3(0), fvec3(1)), fvec4(.5, .5, .5, .5), .03);
    regSphere(s);
}

void init() {
    renderLoad();   // this must be the first, I think
    glfwSetKeyCallback(window, keyCallback);

    mk_433();
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
