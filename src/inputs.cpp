#include "inputs.hpp"
#include <iostream>


int movKeys[6];
int rotKeys[6];

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        movKeys[Rf] = 1;
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        movKeys[Rb] = 1;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        movKeys[Uf] = 1;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        movKeys[Ub] = 1;
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        movKeys[Ff] = 1;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        movKeys[Fb] = 1;

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        rotKeys[Rf] = 1;
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        rotKeys[Rb] = 1;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        rotKeys[Uf] = 1;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        rotKeys[Ub] = 1;
    if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
        rotKeys[Ff] = 1;
    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
        rotKeys[Fb] = 1;

    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        movKeys[Rf] = 0;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        movKeys[Rb] = 0;
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        movKeys[Uf] = 0;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
        movKeys[Ub] = 0;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        movKeys[Ff] = 0;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        movKeys[Fb] = 0;

    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        rotKeys[Rf] = 0;
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        rotKeys[Rb] = 0;
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        rotKeys[Uf] = 0;
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        rotKeys[Ub] = 0;
    if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_RELEASE)
        rotKeys[Ff] = 0;
    if (key == GLFW_KEY_DELETE && action == GLFW_RELEASE)
        rotKeys[Fb] = 0;
}

double getMov(dvec3 &ax) {
    ax = dvec3(movKeys[Rf] - movKeys[Rb], movKeys[Uf] - movKeys[Ub], movKeys[Ff] - movKeys[Fb]);
    if (glm::length(ax) == 0)
        return 0;
    ax = glm::normalize(ax);
    return 2;
}

double getRot(dvec3 &ax) {
    ax = dvec3(rotKeys[Rf] - rotKeys[Rb], rotKeys[Uf] - rotKeys[Ub], rotKeys[Ff] - rotKeys[Fb]);
    if (glm::length(ax) == 0)
        return 0;
    ax = glm::normalize(ax);
    return 2;
}
