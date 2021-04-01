#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <random>
#include <chrono>
#include <iostream>
#include "render.h"
#include "shaderManager.h"
#include "inputs.h"

extern GLFWwindow* window;
using glm::fvec3;
using glm::dvec3;
using glm::dvec4;
using glm::dmat4;

dmat4 player = dmat4(1);
double deltaTime = 0;

dmat4 movMat () {
    dvec3 ax;
    double a = getMov(ax) * deltaTime;
    if (a == 0)
        return dmat4(1);
    double c = cos(a);
    double v = 1 - c;
    double s = sin(a);
    dmat4 m;
    m[0] = dvec4(c, ax * s);
    m[1] = dvec4(-ax.x * s, 1 - ax.x*ax.x*v, -ax.x*ax.y*v, -ax.x*ax.z*v);
    m[2] = dvec4(-ax.y * s, -ax.x*ax.y*v, 1 - ax.y*ax.y*v, -ax.y*ax.z*v);
    m[3] = dvec4(-ax.z * s, -ax.x*ax.z*v, -ax.y*ax.z*v, 1 - ax.z*ax.z*v);
    return player * m * transpose(player);
}

dmat4 rotMat () {
    dvec3 ax;
    double a = getRot(ax) * deltaTime;
    double c = cos(a);
    double v = 1 - c;
    double s = sin(a);
    double x = ax.x, y = ax.y, z = ax.z;
    dmat4 m;
    m[0] = dvec4(1, 0, 0, 0);
    m[1] = dvec4(0, c + v*x*x, v*y*x + s*z, c*z*x - s*y);
    m[2] = dvec4(0, v*x*y - s*z, c + v*y*y, v*z*y + s*x);
    m[3] = dvec4(0, v*x*z + s*y, v*y*z - s*x, c + v*z*z);
    /*
    std::cout << "ax:\n";
    printVector(dvec4(0, ax));
    std::cout << "R:\n";
    printMatrix(m);
    */
    return player * m * transpose(player);
}

void repair (dmat4 &m) {
    m[0] = normalize(m[0]);
    m[1] = normalize(m[1] - m[0] * dot(m[0], m[1]));
    m[2] = normalize(m[2] - m[0] * dot(m[0], m[2]) - m[1] * dot(m[1], m[2]));
    m[3] = normalize(m[3] - m[0] * dot(m[0], m[3]) - m[1] * dot(m[1], m[3]) - m[2] * dot(m[2], m[3]));
}

double drand() {
    return (double) (2 * rand() - 1) / RAND_MAX;
}

double prand() {
    return (double) rand() / RAND_MAX / 4;
}

fvec3 rand3() {
    return glm::normalize(fvec3(prand(), prand(), prand()));
}

dvec4 rand4() {
    return glm::normalize(dvec4(drand(), drand(), drand(), drand()));
}

void init (void) {
    renderLoad();   // this must be the first, I think
    glfwSetKeyCallback(window, keyCallback);
    srand(42);
    for (int i = 0; i < 3; ++i) {
        Figure dot = Figure(rand3(), rand4());
        regFig(dot);
        Figure line = Figure(rand3(), rand4(), rand4());
        regFig(line);
        Figure circle = Figure(BOUND, prand(), rand3(), rand4(), rand4(), rand4());
        regFig(circle);
        Figure sphere = Figure(LIMITED, prand(), rand3(), rand4(), rand4(), rand4(), rand4());
        regFig(sphere);
    }
    /*
    Figure dot = Figure(vec3(1, 0, 1), dvec4(0, 0, 0.1, 1));
    regFig(dot);
    Figure red_line = Figure(vec3(1, 0, 0), dvec4(0, 0, 0.3, 1), dvec4(0, 1, 0.1, 0));
    regFig(red_line);
    Figure green_line = Figure(vec3(0, 1, 0), dvec4(0.1, 0, 0.3, .5), dvec4(0, .1, 0.4, 1));
    regFig(green_line);
    Figure yellow_circle = Figure(BOUND, 0.3, vec3(1, 1, 0), dvec4(0, 0, -0.1, 1), dvec4(.7, 0, .7, 0), dvec4(0, 1, 0, 0));
    regFig(yellow_circle);
    */
}

void loop (void) {
    auto begin = std::chrono::high_resolution_clock::now();
    player = rotMat() * player;
    repair(player);
    player = movMat() * player;
    repair(player);
    setPlayer(player);
    flushKeys();
    renderLoop();   // this must be the last, I think
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration<double>(end - begin);
    deltaTime = std::chrono::duration<double>(elapsed_ms).count();
    // std::cout << "fps: " << 1/deltaTime << '\n';
    //sleep(1);
    /*
    std::cout << '\n';
    printVector(player[0]);
    std::cout << '\n';
    printMatrix(player * transpose(player));
    */
}

int main(void)
{
    init();
    while (!exitCondition()) {
        loop();
    }

    glfwTerminate();
    return 0;
}
