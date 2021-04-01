#include <glm/glm.hpp>
#include <iostream>
#include "render.h"
#include "figure.h"
#include "shaderManager.h"

glm::dvec2 hr = {.5 * PGS_START_WINDOW_WIDTH, .5 * PGS_START_WINDOW_HEIGHT};

GLFWwindow* window;
GLuint VAO, prog;

void renderLoad () {
    if (!glfwInit())
        exit(-1);
    window = glfwCreateWindow(PGS_START_WINDOW_WIDTH, PGS_START_WINDOW_HEIGHT, "Projective Game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL()) {
        std::cout << "Can't load glad\n";
        exit(-1);
    }
    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << '\n';
    glfwSetWindowSizeCallback(window, changeSize);
    prog = loadProgram();
    glUseProgram(prog);
    sendResolution();

    float point[] = {1,1, 1,-1, -1,-1, -1,1};
    GLuint pointsVBO;
    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void renderLoop () {
    glDrawArrays(GL_QUADS, 0, 4);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void sendResolution () {
    GLuint hr_loc = glGetUniformLocation(prog, "hr");
    glUniform2f(hr_loc, (float)hr.x, (float)hr.y);
}

void changeSize (GLFWwindow* _, int w, int h) {
    hr = .5 * glm::dvec2(w, h);
	glViewport(0, 0, w, h);
	sendResolution();
}

bool exitCondition () {
    return glfwWindowShouldClose(window);
}
