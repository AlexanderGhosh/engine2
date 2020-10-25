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
#include "Utils/AssimpWrapper.h"

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
Primative::FrameBuffer frameBuffer;
void saveScreenshotToFile(std::string filename, int windowWidth, int windowHeight, unsigned depthMap);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_W){
        cam.pos += cam.fwd * 0.1f;
    }
}

void saveScreenshotToFile(std::string filename, int windowWidth, int windowHeight, unsigned depthMap) {
    const int numberOfPixels = windowWidth * windowHeight * 3;
    std::vector<unsigned char> pixels;
    pixels.reserve(numberOfPixels);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    // glReadPixels(0, 0, windowWidth, windowHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels.data());
    glGetTexImage(1, 0, GL_RGB_INTEGER, GL_UNSIGNED_BYTE, pixels.data());

    FILE* outputFile = fopen(filename.c_str(), "w");
    short header[] = { 0, 2, 0, 0, 0, 0, (short)windowWidth, (short)windowHeight, 24 };

    fwrite(&header, sizeof(header), 1, outputFile);
    fwrite(pixels.data(), numberOfPixels, 1, outputFile);
    fclose(outputFile);

    printf("Finish writing to file.\n");
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
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
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    glViewport(0, 0, 800, 600);
    glewInit();

    cam.pos = { 0, 0.5, 0.25 };

    ResourceLoader::createShader("Basics/DefaultShader");
    ResourceLoader::createShader("Basics/QuadShader");
    // cube
    /*Primative::Mesh* m = new Primative::Mesh();
    Primative::Vertex v1({ -0.5, -0.5, -0.5 }, { 0, 0 });
    Primative::Vertex v2({ 0.5, -0.5, -0.5 }, { 1, 0 });
    Primative::Vertex v3({ 0.5, 0.5, -0.5 }, { 1, 1 });
    Primative::Vertex v4({ -0.5, 0.5, -0.5 }, { 0, 1 });
    Primative::Vertex v5({ -0.5, -0.5, 0.5 }, { 0, 0 });
    Primative::Vertex v6({ 0.5, -0.5, 0.5 }, { 1, 0 });
    Primative::Vertex v7({ 0.5, 0.5, 0.5 }, { 1, 1 });
    Primative::Vertex v8({ -0.5, 0.5, 0.5 }, { 0, 1 });

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
    /*Primative::Mesh* m = new Primative::Mesh();
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
    };*/

    AssimpWrapper w;
    auto t = w.loadModel("C:/Users/AGWDW/Desktop/blend/Handgun_Game_Blender Gamer Engine.obj");

    unsigned tex1 = ResourceLoader::createTexture("Basics/Textures/handgun_C.jpg", TextureType::DiffuseMap, 0);
    unsigned tex2 = ResourceLoader::createTexture("Basics/Textures/handgun_S.jpg", TextureType::SpecularMap, 0);
    unsigned tex3 = ResourceLoader::createTexture("Basics/Textures/handgun_N.jpg", TextureType::NormalMap, 0);
    // unsigned tex = ResourceLoader::createTexture("Basics/Textures/wood.jpg", TextureType::DiffuseMap);

    Render::RenderMesh* r = new Render::RenderMesh();
    // std::cout << t.size() << std::endl;
    t.pop_back();
    t.pop_back();
    for (auto& m : t) {
        r->addMesh(m);
        delete m;
        m = nullptr;
    }
    r->setShader("DefaultShader");


    Primative::Mesh* quad_mesh = new Primative::Mesh();
    quad_mesh->verts.push_back(Primative::Vertex({ -1, -1, 0 }, { 0, 0 }));
    quad_mesh->verts.push_back(Primative::Vertex({ -1, 1, 0 }, { 0, 1 }));
    quad_mesh->verts.push_back(Primative::Vertex({ 1, -1, 0 }, { 1, 0 }));
    quad_mesh->verts.push_back(Primative::Vertex({ 1, 1, 0 }, { 1, 1 }));
    
    quad_mesh->indices = {
        0, 1, 2, 3
    };
    
    Render::RenderMesh* quad_r = new Render::RenderMesh();
    quad_r->addMesh(quad_mesh, GL_TRIANGLE_STRIP);
    delete quad_mesh;
    quad_mesh = nullptr;
    quad_r->setShader("QuadShader");// QuadShader
    
    GameObject quad;
    quad.addComponet(quad_r);
    
    GameObject obj;
    obj.getTransform()->Position = { 0, 0, 0 };
    obj.addComponet(r);

    Primative::StaticBuffer b;
    // view    | matrix
    // proj    | matrix
    // viewPos | vec
    // gamma   | float
    b.init(2 * sizeof(mat4) + sizeof(vec3) + sizeof(float), 0);


    mat4 projection = perspective(glm::radians(cam.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

    b.fill(sizeof(mat4), sizeof(mat4), value_ptr(projection));

    float gamma = 2.2f;
    b.fill(sizeof(mat4) * 2 + sizeof(vec3), sizeof(float), &gamma);

    short tick = 0;
    float lastTime = glfwGetTime();
    float deltaTime = 0;
    unsigned fps = 0;

    frameBuffer = Primative::FrameBuffer({ "depth" }, { 800, 600 });

    while (!glfwWindowShouldClose(window))
    {
        // frameBuffer.bind();
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        fps = 1.0f / deltaTime;
        lastTime = currentTime;
        // std::cout << fps << std::endl;

        b.fill(0, sizeof(mat4), value_ptr(cam.getView()));
        b.fill(sizeof(mat4) * 2, sizeof(vec3), value_ptr(cam.getPos()));

        glClearColor(0.5, 1, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // new fbo
        // glBindFramebuffer(GL_FRAMEBUFFER, 1);
        frameBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        obj.tick(++tick % FIXED_UPDATE_RATE);
        
        // default fbo
        frameBuffer.unBind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, frameBuffer.getTextureId("depth"));
        
        quad.tick(++tick % FIXED_UPDATE_RATE);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete r;
    ResourceLoader::cleanUp(); 
    frameBuffer.cleanUp();
    glfwTerminate();
    // delete window;
    return 0;
}