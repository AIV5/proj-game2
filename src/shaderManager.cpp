#include <cstdio>
#include <cstring>
#include <iostream>
#include <glad/glad.h>
#include "settings.hpp"
#include "shaderManager.hpp"

extern GLuint prog;
int faceCount = 0;

void _errorDegenerate () {
    std::cerr << "Error when constructing a face. Degenerate vector sistem was given.\n";
    exit(-1);
}

void loadShader (const char fileName[], GLuint shader) {
    char* code = (char *)malloc(PGS_SHADER_FILE_SIZE);
    if (code == nullptr) {
        std::cerr << "Error when loading shader. Can't allocate memory\n";
        exit(-1);
    }
    memset(code, 0, PGS_SHADER_FILE_SIZE);
    FILE *fin = fopen(fileName, "rb");
    if (fin) {
        fread(code, 1, PGS_SHADER_FILE_SIZE, fin);
        fclose(fin);
    } else {
        std::cerr << "Error when loading shader. File " << fileName << " not found\n";
        free(code);
        exit(-1);
    }
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    free(code);

    GLint ok;
    GLchar log[2000];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader, 2000, NULL, log);
        std::cerr << "Error when compiling shader " << fileName << "\nlog:\n" << log << "\n";
        exit(-1);
    }
}

GLuint loadProgram () {
    GLuint prog = glCreateProgram();
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    loadShader(PGS_FRAGMENT_SHADER_NAME, fragShader);
    glAttachShader(prog, fragShader);
    glDeleteShader(fragShader);
    glLinkProgram(prog);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(prog, 2000, NULL, log);
        std::cerr << "Error when linking program\nlog:\n" << log << "\n";
        exit(-1);
    }
    return prog;
}

GLuint getArrayLoc (const char name[], int index) {
    char text [32];
    sprintf(text, "%s[%d]", name, index);
    return glGetUniformLocation(prog, text);
}

void printMatrix (dmat4 m) {
    std::cout << m[0][0] << ' ' << m[1][0] << ' ' << m[2][0] << ' ' << m[3][0] << '\n';
    std::cout << m[0][1] << ' ' << m[1][1] << ' ' << m[2][1] << ' ' << m[3][1] << '\n';
    std::cout << m[0][2] << ' ' << m[1][2] << ' ' << m[2][2] << ' ' << m[3][2] << '\n';
    std::cout << m[0][3] << ' ' << m[1][3] << ' ' << m[2][3] << ' ' << m[3][3] << '\n';
}

void printVector (dvec4 v) {
    std::cout << v[0] << ' ' << v[1] << ' ' << v[2] << ' ' << v[3] << '\n';
}

int regFace (Face &face) {
    face.faceIndex = faceCount;
    GLuint faceNumberLoc = glGetUniformLocation(prog, "faceNumber");
    GLuint faceIMLoc = getArrayLoc("faceIM", faceCount);
    GLuint faceColorLoc = getArrayLoc("faceColor", faceCount);
    GLuint faceCenterLoc = getArrayLoc("faceCenter", faceCount);
    GLuint faceStartLoc = getArrayLoc("faceStart", faceCount);
    GLuint facePointLoc = getArrayLoc("facePoint", faceCount);
    GLuint faceRadLoc = getArrayLoc("faceRad", faceCount);
    ++faceCount;
    glUniform1i(faceNumberLoc, faceCount);
    glUniform4f(faceColorLoc, face.faceColor[0], face.faceColor[1], face.faceColor[2], 1);
    fvec4 center = fvec4 (face.faceCenter);
    glUniform4fv(faceCenterLoc, 1, &center[0]);
    fvec4 start = fvec4 (face.faceStart);
    glUniform4fv(faceStartLoc, 1, &start[0]);
    fvec4 point = fvec4 (face.facePoint);
    glUniform4fv(facePointLoc, 1, &point[0]);
    glUniform1f(faceRadLoc, (float)face.faceRad);

    std::cout << "center: ";
    printVector(center);
    std::cout << "start: ";
    printVector(start);
    std::cout << "point: ";
    printVector(point);

    return face.faceIndex;
}

void setPlayer (dmat4 coord) {
    GLuint PLoc = glGetUniformLocation(prog, "playerP");
    GLuint RLoc = glGetUniformLocation(prog, "playerR");
    GLuint ULoc = glGetUniformLocation(prog, "playerU");
    GLuint FLoc = glGetUniformLocation(prog, "playerF");
    fvec4 Pf = fvec4 (coord[0]);
    fvec4 Rf = fvec4 (coord[1]);
    fvec4 Uf = fvec4 (coord[2]);
    fvec4 Ff = fvec4 (coord[3]);
    glUniform4fv(PLoc, 1, &Pf[0]);
    glUniform4fv(RLoc, 1, &Rf[0]);
    glUniform4fv(ULoc, 1, &Uf[0]);
    glUniform4fv(FLoc, 1, &Ff[0]);
}
