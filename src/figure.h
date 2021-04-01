#pragma once

#include <glm/glm.hpp>

#define PGS_MIN_VECTOR_LENGTH 0.001f

using glm::fvec3;
using glm::dvec4;
using glm::dmat4;

enum _objType_t {FULL, LIMITED, BOUND};
const dvec4 nullVec = dvec4(0, 0, 0, 0);

class Figure {
private:
    void init (fvec3 color, dvec4 p, dvec4 r, dvec4 u, dvec4 f);
public:
    int objIndex;
    int objType;
    fvec3 objColor;
    double objRad;
    dmat4 objCoord;
    Figure(void);
    Figure(fvec3 color, dvec4 p, dvec4 r=nullVec, dvec4 u=nullVec, dvec4 f=nullVec);
    Figure(_objType_t type, double rad, fvec3 color, dvec4 p, dvec4 r=nullVec, dvec4 u=nullVec, dvec4 f=nullVec);
};

dvec4 orthonormal (dvec4 f, dvec4 u=nullVec, dvec4 r=nullVec, dvec4 p=nullVec);
