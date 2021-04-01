#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using glm::dvec3;

enum _key_code_t {Rf, Rb, Uf, Ub, Ff, Fb};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void flushKeys ();

double getMov(dvec3 &ax);
double getRot(dvec3 &ax);
