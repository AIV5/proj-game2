#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "settings.h"

void renderLoad ();
void renderLoop ();

void sendResolution ();
void changeSize (GLFWwindow* _, int w, int h);

bool exitCondition ();
