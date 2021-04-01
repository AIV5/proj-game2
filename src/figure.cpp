#include <iostream>
#include "figure.h"

void _errorMinVectorLength () {
    std::cout << "Error when constructing a figure. Degenerate vector sistem was given.\n";
    exit(-1);
}

Figure::Figure(void) {
    std::cout << "Error when constructing a figure. Empty constructor called.";
}

Figure::Figure(fvec3 color, dvec4 p, dvec4 r, dvec4 u, dvec4 f) {
    init(color, p, r, u, f);
    objType = FULL;
}


Figure::Figure(_objType_t type, double rad, fvec3 color, dvec4 p, dvec4 r, dvec4 u, dvec4 f) {
    init(color, p, r, u, f);
    objType = type;
    objRad = rad;
}


dvec4 orthonormal (dvec4 f, dvec4 u, dvec4 r, dvec4 p) {
    if (f == nullVec)
        return nullVec;
    if (glm::length(f) < PGS_MIN_VECTOR_LENGTH)
        _errorMinVectorLength();
    f = glm::normalize(f);
    f = f - p * glm::dot(p, f) - r * glm::dot(r, f)  - u * glm::dot(u, f);
    if (glm::length(f) < PGS_MIN_VECTOR_LENGTH)
        _errorMinVectorLength();
    f = glm::normalize(f);
    return f;
}

void Figure::init (fvec3 color, dvec4 p, dvec4 r, dvec4 u, dvec4 f) {
    objIndex = -1;
    objColor = color;
    p = orthonormal(p);
    if (p == nullVec)
        _errorMinVectorLength();
    r = orthonormal(r, p);
    if (r == nullVec) {
        if (u != nullVec || f != nullVec)
            _errorMinVectorLength();
    } else {
        u = orthonormal(u, r, p);
        if (u == nullVec) {
            if (f != nullVec)
            _errorMinVectorLength();
        } else {
            f = orthonormal(f, u, r, p);
        }
    }
    objCoord[0] = p;
    objCoord[1] = r;
    objCoord[2] = u;
    objCoord[3] = f;
}
