#include <iostream>
#include "face.hpp"
#include "settings.hpp"

Face::Face() {
    std::cerr << "Warning when constructing a face. Empty constructor called.";
}

Face::Face(fvec3 color, vector<fvec4> &vertexes)
    : faceColor(color), faceVert(vertexes) {
    faceIndex = -1;
}
