#pragma once

#include <glm/glm.hpp>

using glm::fvec3;
using glm::dvec4;
using glm::dmat4;

class Face {
public:
    int faceIndex;
    fvec3 faceColor;
    int faceVertexNumber;
    dvec4 faceCenter;
    dvec4 faceStart;
    dvec4 facePoint;
    double faceRad;
    Face(void);
    Face(fvec3 color, int n, dvec4 center, dvec4 start, dvec4 point, double rad);
};
