#include <iostream>
#include "face.hpp"
#include "settings.hpp"

dvec4 _nullVec = dvec4(0);

void _errorDegenerate () {
    std::cerr << "Error when constructing a face. Degenerate vector sistem was given.\n";
    exit(-1);
}

void _orthonormalize (dvec4 &target, const dvec4 &u=_nullVec, const dvec4 &v=_nullVec) {
    if (glm::length(target) < PGS_EPSILON)
        _errorDegenerate();
    target = glm::normalize(target);
    target -= v * glm::dot(target, v) + u * glm::dot(target, u);
    if (glm::length(target) < PGS_EPSILON)
        _errorDegenerate();
    target = glm::normalize(target);
}

Face::Face() {
    std::cerr << "Warning when constructing a face. Empty constructor called.";
}

Face::Face(fvec3 color, int n, dvec4 center, dvec4 start, dvec4 point, double rad) {
    _orthonormalize(center);
    _orthonormalize(start, center);
    _orthonormalize(point, start, center);
    faceColor = color;
    faceVertexNumber = n;
    faceCenter = center;
    faceStart = start;
    facePoint = point;
    faceRad = rad;
}
