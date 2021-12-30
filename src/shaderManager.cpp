#include <cstdio>
#include <cstring>
#include <iostream>
#include <glad/glad.h>
#include "settings.hpp"
#include "shaderManager.hpp"

extern GLuint prog;
int faceCount = 0;
int vertCount = 0;
int sphereCount = 0;

void _errorDegenerate() {
    std::cerr << "Error when constructing a face. Degenerate vector sistem was given.\n";
    exit(-1);
}

void loadShader(const char fileName[], GLuint shader) {
    char *code = (char *) malloc(PGS_SHADER_FILE_SIZE);
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
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    free(code);

    GLint ok;
    GLchar log[2000];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader, 2000, nullptr, log);
        std::cerr << "Error when compiling shader " << fileName << "\nlog:\n" << log << "\n";
        exit(-1);
    }
}

GLuint loadProgram() {
    GLuint program = glCreateProgram();
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    loadShader(PGS_FRAGMENT_SHADER_NAME, fragShader);
    glAttachShader(program, fragShader);
    glDeleteShader(fragShader);
    glLinkProgram(program);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(program, 2000, nullptr, log);
        std::cerr << "Error when linking program\nlog:\n" << log << "\n";
        exit(-1);
    }
    return program;
}

GLint getArrayLoc(const char name[], int index) {
    char text[32];
    sprintf(text, "%s[%d]", name, index);
    return glGetUniformLocation(prog, text);
}

//GLint get2dArrayLoc(const char name[], int i, int j) {
//    char text[32];
//    sprintf(text, "%s[%d][%d]", name, i, j);
//    return glGetUniformLocation(prog, text);
//}

void printMatrix(dmat4 m) {
    std::cout << m[0][0] << ' ' << m[1][0] << ' ' << m[2][0] << ' ' << m[3][0] << '\n';
    std::cout << m[0][1] << ' ' << m[1][1] << ' ' << m[2][1] << ' ' << m[3][1] << '\n';
    std::cout << m[0][2] << ' ' << m[1][2] << ' ' << m[2][2] << ' ' << m[3][2] << '\n';
    std::cout << m[0][3] << ' ' << m[1][3] << ' ' << m[2][3] << ' ' << m[3][3] << '\n';
}

void printVector(dvec4 v) {
    std::cout << v[0] << ' ' << v[1] << ' ' << v[2] << ' ' << v[3] << '\n';
}

int regFace(Face &face) {
    if (faceCount + 1 >= PGS_MAX_FACE) {
        std::cerr << "Exceeded the limit of faces\n";
        return -1;
    } else if (vertCount + face.faceVert.size() >= PGS_MAX_VERT) {
        std::cerr << "Exceeded the limit of vertexes\n";
        return -1;
    }
    face.faceIndex = faceCount;
    auto faceNumberLoc = glGetUniformLocation(prog, "faceNumber");
    auto faceColorLoc = getArrayLoc("faceColor", faceCount);
    auto faceVertNumLoc = getArrayLoc("faceVertNum", faceCount);
    auto faceVertStartLoc = getArrayLoc("faceVertStart", faceCount);
    auto VertLoc = getArrayLoc("Vert", vertCount);
    ++faceCount;
    glUniform1i(faceNumberLoc, faceCount);
    glUniform4f(faceColorLoc, face.faceColor[0], face.faceColor[1], face.faceColor[2], 1);
    for (auto v: face.faceVert) {
        printVector(v);
    }
    std::cout << std::endl;
    glUniform1i(faceVertStartLoc, vertCount);
    vertCount += face.faceVert.size();
    glUniform1i(faceVertNumLoc, face.faceVert.size());
    glUniform4fv(VertLoc, face.faceVert.size(), &face.faceVert[0][0]);
    return face.faceIndex;
}

int regSphere(Sphere &sphere) {
    if (sphereCount + 1 >= PGS_MAX_SPHERE) {
        std::cerr << "Exceeded the limit of spheres\n";
        return -1;
    }
    sphere.sphereIndex = sphereCount;
    auto sphereNumberLoc = glGetUniformLocation(prog, "sphereNumber");
    auto sphereColorLoc = getArrayLoc("sphereColor", sphereCount);
    auto sphereCenterLoc = getArrayLoc("sphereCenter", sphereCount);
    auto sphereRadLoc = getArrayLoc("sphereRad", sphereCount);
    ++sphereCount;
    glUniform1i(sphereNumberLoc, sphereCount);
    glUniform4f(sphereColorLoc, sphere.sphereColor[0], sphere.sphereColor[1], sphere.sphereColor[2], 1);
    glUniform4f(sphereCenterLoc, sphere.sphereCenter[0], sphere.sphereCenter[1], sphere.sphereCenter[2], sphere.sphereCenter[3]);
    glUniform1f(sphereRadLoc, sphere.sphereRad);
    return sphere.sphereIndex;
}

void setPlayer(dmat4 coord) {
    auto PLoc = glGetUniformLocation(prog, "playerP");
    auto RLoc = glGetUniformLocation(prog, "playerR");
    auto ULoc = glGetUniformLocation(prog, "playerU");
    auto FLoc = glGetUniformLocation(prog, "playerF");
    auto Pf = fvec4(coord[0]);
    auto Rf = fvec4(coord[1]);
    auto Uf = fvec4(coord[2]);
    auto Ff = fvec4(coord[3]);
    glUniform4fv(PLoc, 1, &Pf[0]);
    glUniform4fv(RLoc, 1, &Rf[0]);
    glUniform4fv(ULoc, 1, &Uf[0]);
    glUniform4fv(FLoc, 1, &Ff[0]);
}
