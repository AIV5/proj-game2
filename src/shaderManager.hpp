#pragma once

#include "face.hpp"

using glm::fmat4;
using glm::dmat4;
using glm::fvec4;
using glm::dvec4;

void loadShader (char *fileName, GLint shader);
GLuint loadProgram (void);

GLuint getArrayLoc (char* name, int index);

int regFace (Face &face);

void setPlayer (dmat4 coord);

void printMatrix (dmat4 m);
void printVector (dvec4 m);
