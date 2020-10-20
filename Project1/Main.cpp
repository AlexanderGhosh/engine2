#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Rendering/Engine.h"
#include "Rendering/Shading/Manager.h"
#include "Utils/ResourceLoader.h"
#include "Utils/Camera.h"
#include "GameObject/GameObject.h"

Utils::Camera cam;
bool firstMouse = 1;
float lastX = 0, lastY = 0;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = 0;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    cam.ProcessMouseMovement(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_W){
        cam.pos += cam.fwd * 0.1f;
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    glViewport(0, 0, 800, 600);
    glewInit();


    string name = ResourceLoader::createShader("Basics/DefaultShader");

    // cube
    /*Primative::Mesh* m = new Primative::Mesh();
    Primative::Vertex v1({ -0.5, -0.5, -0.5 });
    Primative::Vertex v2({ 0.5, -0.5, -0.5 });
    Primative::Vertex v3({ 0.5, 0.5, -0.5 });
    Primative::Vertex v4({ -0.5, 0.5, -0.5 });
    Primative::Vertex v5({ -0.5, -0.5, 0.5 });
    Primative::Vertex v6({ 0.5, -0.5, 0.5 });
    Primative::Vertex v7({ 0.5, 0.5, 0.5 });
    Primative::Vertex v8({ -0.5, 0.5, 0.5 });

    m->verts.push_back(v1);
    m->verts.push_back(v2);
    m->verts.push_back(v3);
    m->verts.push_back(v4);
    m->verts.push_back(v5);
    m->verts.push_back(v6);
    m->verts.push_back(v7);
    m->verts.push_back(v8);

    m->indices = {
        0, 1, 3, 3, 1, 2,
        1, 5, 2, 2, 5, 6,
        5, 4, 6, 6, 4, 7,
        4, 0, 7, 7, 0, 3,
        3, 2, 7, 7, 2, 6,
        4, 5, 0, 0, 5, 1
    };*/

    // pyramid
    Primative::Mesh* m = new Primative::Mesh();
    Primative::Vertex v1({ -0.5, -0.5, -0.5 });
    Primative::Vertex v2({ 0.5, -0.5, -0.5 });
    Primative::Vertex v3({ 0.5, 0.5, -0.5 });
    Primative::Vertex v4({ -0.5, 0.5, -0.5 });
    Primative::Vertex v5({ 0, 0, 0.5 });

    m->verts.push_back(v1);
    m->verts.push_back(v2);
    m->verts.push_back(v3);
    m->verts.push_back(v4);
    m->verts.push_back(v5);

    m->indices = {
        0, 1, 3, 3, 1, 2,
        0, 4, 1,
        0, 4, 3,
        2, 4, 1,
        2, 4, 3
    };

    Render::RenderMesh* r = new Render::RenderMesh();
    r->addMesh(m);
    r->setShader("default");

    delete m;


    GameObject obj;
    obj.addComponet(r);


    Primative::StaticBuffer b;
    b.init(2 * sizeof(mat4), 0);


    mat4 projection = perspective(glm::radians(cam.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

    b.fill(sizeof(mat4), sizeof(mat4), value_ptr(projection));

    short tick = 0;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1, 1, 1, 0.5);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        /*Render::Shading::Manager::setActive(shaderProgram);
        Render::Shading::Manager::setValue("col", { 1, 0, 1 });*/
        /*b.fill(0, sizeof(float), &c);
        b.fill(sizeof(float), sizeof(float), &c);
        b.fill(2 * sizeof(float), sizeof(float), &c);*/

        b.fill(0, sizeof(mat4), value_ptr(cam.getView()));
        obj.tick(++tick % FIXED_UPDATE_RATE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete r;
    ResourceLoader::cleanUp();
    glfwTerminate();
    delete window;
    return 0;
}