cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(PROJECT_NAME ProjGame2)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
            src/main.cpp
            src/settings.hpp
            src/shaderManager.hpp
            src/shaderManager.cpp
            src/render.hpp
            src/render.cpp
            src/inputs.hpp
            src/inputs.cpp
            src/face.hpp
            src/face.cpp
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

