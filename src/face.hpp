#pragma once

#include <glm/glm.hpp>
#include <vector>

using glm::fvec3;
using glm::fvec4;
using glm::dmat4;
using std::vector;

class Face {
public:
    int faceIndex;
    fvec3 faceColor;
    vector<fvec4> faceVert;
    Face();
    Face(fvec3 color, vector<fvec4> &vertexes);
};
