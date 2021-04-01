#include "inputs.h"
#include <iostream>


int movKeys[6];
int rotKeys[6];

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_D && action == GLFW_REPEAT | GLFW_PRESS)
        movKeys[Rf] = 1;
    if (key == GLFW_KEY_A && action == GLFW_REPEAT | GLFW_PRESS)
        movKeys[Rb] = 1;
    if (key == GLFW_KEY_SPACE && action == GLFW_REPEAT | GLFW_PRESS)
        movKeys[Uf] = 1;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_REPEAT | GLFW_PRESS)
        movKeys[Ub] = 1;
    if (key == GLFW_KEY_W && action == GLFW_REPEAT | GLFW_PRESS)
        movKeys[Ff] = 1;
    if (key == GLFW_KEY_S && action == GLFW_REPEAT | GLFW_PRESS)
        movKeys[Fb] = 1;

    if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT | GLFW_PRESS)
        rotKeys[Rf] = 1;
    if (key == GLFW_KEY_UP && action == GLFW_REPEAT | GLFW_PRESS)
        rotKeys[Rb] = 1;
    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT | GLFW_PRESS)
        rotKeys[Uf] = 1;
    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT | GLFW_PRESS)
        rotKeys[Ub] = 1;
    if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_REPEAT | GLFW_PRESS)
        rotKeys[Ff] = 1;
    if (key == GLFW_KEY_DELETE && action == GLFW_REPEAT | GLFW_PRESS)
        rotKeys[Fb] = 1;
}

void flushKeys () {
    movKeys[0] = 0;
    movKeys[1] = 0;
    movKeys[2] = 0;
    movKeys[3] = 0;
    movKeys[4] = 0;
    movKeys[5] = 0;

    rotKeys[0] = 0;
    rotKeys[1] = 0;
    rotKeys[2] = 0;
    rotKeys[3] = 0;
    rotKeys[4] = 0;
    rotKeys[5] = 0;
}

double getMov(dvec3 &ax) {
    ax = dvec3(movKeys[Rf] - movKeys[Rb], movKeys[Uf] - movKeys[Ub], movKeys[Ff] - movKeys[Fb]);
    if (glm::length(ax) == 0)
        return 0;
    ax = glm::normalize(ax);
    return .5;
}

double getRot(dvec3 &ax) {
    ax = dvec3(rotKeys[Rf] - rotKeys[Rb], rotKeys[Uf] - rotKeys[Ub], rotKeys[Ff] - rotKeys[Fb]);
    if (glm::length(ax) == 0)
        return 0;
    ax = glm::normalize(ax);
    return .5;
}
