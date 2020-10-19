#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <string>

#include <glm.hpp>

#include "Rendering/Engine.h"
#include "Rendering/Shading/Manager.h"
#include "Utils/ResourceLoader.h"

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glViewport(0, 0, 800, 600);
    glewInit();


    string name = ResourceLoader::createShader("Basics/DefaultShader");
    const unsigned& shaderProgram = ResourceLoader::getShader("fhseiu");

    Primative::Mesh* m = new Primative::Mesh();
    Primative::Vertex v1({ 0.5, 0.5, 0 });
    Primative::Vertex v2({ 0.5, -0.5, 0 });
    Primative::Vertex v3({ -0.5, -0.5, 0 });
    Primative::Vertex v4({ -0.5, 0.5, 0 });
    m->verts.push_back(v1);
    m->verts.push_back(v2);
    m->verts.push_back(v3);
    m->verts.push_back(v4);

    m->indices = {
        0, 1, 3,
        1, 2, 3
    };

    Render::Rendering r;
    r.addMesh(m);

    delete m;

    Primative::StaticBuffer b;
    b.init(sizeof(vec3), 1);
    b.fill(0, sizeof(vec3), value_ptr(vec3(0, 1, 1)));

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1, 1, 1, 0.5);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        /*Render::Shading::Manager::setActive(shaderProgram);
        Render::Shading::Manager::setValue("col", { 1, 0, 1 });*/
        /*b.fill(0, sizeof(float), &c);
        b.fill(sizeof(float), sizeof(float), &c);
        b.fill(2 * sizeof(float), sizeof(float), &c);*/
        r.draw(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}