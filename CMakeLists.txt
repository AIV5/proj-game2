cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(PROJECT_NAME ProjGame)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
            src/main.cpp
            src/settings.h
            src/shaderManager.cpp
            src/shaderManager.h
            src/render.cpp
            src/render.h
            src/figure.cpp
            src/figure.h
            src/inputs.cpp
            src/inputs.h
            )

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(external/glm)

