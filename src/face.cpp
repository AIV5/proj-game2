#include <iostream>
#include "face.hpp"
#include "settings.hpp"

Face::Face() {
    std::cerr << "Warning when constructing a face. Empty constructor called.";
}

Face::Face(fvec3 color, int n, dvec4 center, dvec4 start, dvec4 point, double rad)
    : faceColor(color), faceVertexNumber(n), faceCenter(center), faceStart(start), facePoint(point), faceRad(rad) {
}
