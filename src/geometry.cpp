#include <iostream>
#include "geometry.hpp"
#include "settings.hpp"

Face::Face() {
    std::cerr << "Warning when constructing a face. Empty constructor called.";
}

Face::Face(fvec3 color, vector<fvec4> &vertexes)
        : faceColor(color), faceVert(vertexes) {
    faceIndex = -1;
}

Sphere::Sphere() {
    std::cerr << "Warning when constructing a sphere. Empty constructor called.";
}

Sphere::Sphere(fvec3 color, fvec4 center, double rad)
        : sphereColor(color), sphereCenter(center), sphereRad(rad) {
    sphereIndex = -1;
}