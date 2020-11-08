#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Rendering/Rendering.h"
#include "Rendering/Shading/Manager.h"
#include "SoundManager.h"
#include "Utils/ResourceLoader.h"
#include "GameObject/GameObject.h"
#include "Scene/GameScene.h"
#include "Utils/AssimpWrapper.h"
#include "UI/Renderer.h"
#include "EventSystem/Handler.h"
#include "UI/TextRenderer.h"
#include "UI/Elements/TextField.h"
#include "Componets/AudioSource.h"
#include "Componets/Camera.h"

#define PBRen 1

Componet::Camera cam;
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
void saveScreenshotToFile(std::string filename, int windowWidth, int windowHeight, unsigned depthMap);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_W){
        cam.setPos(cam.getForward() * 0.1f + cam.getForward());
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

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    glViewport(0, 0, 800, 600);
    glewInit();

    cam.setPos({ 0, 200, 700 });

    ResourceLoader::createShader("Basics/DefaultShader");
    ResourceLoader::createShader("Basics/ShadowShader");
    ResourceLoader::createShader("Basics/QuadShader");
    ResourceLoader::createShader("Basics/PBRShader");
    ResourceLoader::createShader("Basics/UIShader");
    ResourceLoader::createShader("Basics/TextShader");
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

    // auto t = w.loadModel("C:/Users/AGWDW/Desktop/blend/Handgun_Game_Blender Gamer Engine.obj");
    // auto t = w.loadModel("Basics/Models/sword.obj");
    auto pistolBuffers = ResourceLoader::createModel("Basics/Models/pistol.obj");
    // auto t = FileReaders::AssimpWrapper::loadModel("Basics/Models/pistol.obj");

    // ResourceLoader::createTexture("Basics/Textures/handgun_C.jpg", TextureType::DiffuseMap, 0); // diff
    // ResourceLoader::createTexture("Basics/Textures/handgun_S.jpg", TextureType::SpecularMap, 0); // spec
    // ResourceLoader::createTexture("Basics/Textures/handgun_N.jpg", TextureType::NormalMap, 0); // norm

    // ResourceLoader::createTexture("Basics/Textures/NazghulGreatsword_BaseColor.png", TextureType::AlbedoMap, 0); // albedo
    // ResourceLoader::createTexture("Basics/Textures/NazghulGreatsword_Metallic.png", TextureType::MetalicMap, 0); // metalic
    // ResourceLoader::createTexture("Basics/Textures/NazghulGreatsword_NormalOpenGL.png", TextureType::NormalMap, 0); // norm
    // ResourceLoader::createTexture("Basics/Textures/NazghulGreatsword_AmbientOcclusion.png", TextureType::AOMap, 0); // ao
    // ResourceLoader::createTexture("Basics/Textures/NazghulGreatsword_Roughness.png", TextureType::RoughnessMap, 0); // roughness

    auto tjiol = ResourceLoader::createTexture("Basics/Textures/Base_color 1.jpg", TextureType::AlbedoMap, 0); // albedo
    ResourceLoader::createTexture("Basics/Textures/metal.jpg", TextureType::MetalicMap, 0); // metalic
    ResourceLoader::createTexture("Basics/Textures/handgun_N.jpg", TextureType::NormalMap, 0); // norm
    ResourceLoader::createTexture("Basics/Textures/gloss.jpg", TextureType::AOMap, 0); // ao
    ResourceLoader::createTexture("Basics/Textures/rough.jpg", TextureType::RoughnessMap, 0); // roughness


    // unsigned tex = ResourceLoader::createTexture("Basics/Textures/wood.jpg", TextureType::DiffuseMap);

    Render::RenderMesh* r = new Render::RenderMesh();
    r->addMesh(pistolBuffers);
    Materials::Base* mat = new Materials::PBR({ 1 }, { 3 }, { 2 }, { 5 }, { 4 }); // new Materials::PBR({ 1 }, { 3 }, { 2 }, { 5 }, { 4 });
    if (!PBRen) {
        mat = new Materials::Forward({ 1 }, { 2 }, { 3 }, 32);
    }
    r->setMaterial(mat);
    
    GameObject* gun = new GameObject();
    gun->getTransform()->Position = { 0, 0, 0 };
    gun->addComponet(r);

    Primative::StaticBuffer b("m4, m4, v3, f, m4", 0);
    // view    | matrix 4
    // proj    | matrix 4
    // viewPos | vector 3
    // gamma   | scalar f
    // lspaceM | matrix 4

    mat4 projection = perspective(glm::radians(cam.getFOV()), 800.0f / 600.0f, 0.01f, 1000.0f);

    b.fill(1, value_ptr(projection));

    float gamma = 2.2f;
    b.fill(3, &gamma);

    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

    glm::mat4 lightView = glm::lookAt(glm::vec3(10, 10 ,10),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
   
    b.fill(4, value_ptr(lightSpaceMatrix));

    short tick = 0;
    float lastTime = glfwGetTime();
    float deltaTime = 0;
    unsigned fps = 0;

    Primative::FrameBuffer* frameBuffer = new Primative::FrameBuffer({ "depth" }, { 800, 600 });

    GameScene scene;
    scene.addFBO("shadows", frameBuffer);
    scene.addPreProcLayer("shadows", ResourceLoader::getShader("ShadowShader"));
    scene.setPostProcShader(ResourceLoader::getShader(PBRen ? "PBRShader" : "DefaultShder")); // PBRShader
    scene.addObject(gun);

    // UI //
    UI::Renderer::init(ResourceLoader::getShader("UIShader"), { 800, 600 });

    UI::TextField element;
    element.setText("Hello World");
    element.setForgroundColor({ 1, 1, 0 });
    // element.toggleSelected();
    // element.setBackgroundImage(1);
    element.setPos({ 400, 300 });
    element.setWidth(250);
    element.setHeight(40);

    element.mouseDown = [](UI::Element* sender) {
        std::cout << "mouse down\n";
    };
    element.mouseUp = [](UI::Element* sender) {
        std::cout << "mouse up\n";
    };

    UI::Pane pane;
    pane.addElement(&element);
    // UI //

    // SOUNDS //
    SoundManager::init();
    const auto buffer = SoundManager::createBuffer("C:/Users/AGWDW/Desktop/iamtheprotectorofthissystem.wav");
    Componet::AudioSource* audio = new Componet::AudioSource(
        SoundManager::createSoundSource());
    audio->addBuffer(buffer);

    Events::Handler::init(window);

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        fps = 1.0f / deltaTime;
        lastTime = currentTime;

        // std::cout << fps << std::endl; // FPS Count

        b.fill(0, value_ptr(cam.getView()));
        b.fill(2, value_ptr(cam.getPos()));


        // scene.preProcess(); // shadows
        scene.postProcess();// render to screen

        /*glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        UI::Renderer::render(&pane);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        pane.update(); // events check
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        textR.drawText("Hello", 25, 25, 1, { 1, 0, 0 });*/

        if (Events::Handler::getKey(Events::Key::Space, Events::Action::Down)) {
            audio->play();
        }
        if (Events::Handler::getKey(Events::Key::R_Shift, Events::Action::Down)) {
            audio->pause();
        }
        if (Events::Handler::getKey(Events::Key::Escape, Events::Action::Down)) {
            break;
        }
        // sound->update();

        glfwSwapBuffers(window);
        Events::Handler::pollEvents();
    }
    Render::Shading::Manager::cleanUp(); // deactivats shdaer

    delete mat;

    scene.cleanUp(); // removes and destrys all componets and fbos (destroysing comonets doesnt destry buffers(except audio source))

    b.cleanUp(); // destroys UBO 0
    UI::TextRenderer::cleanUpStatic(); // destroys char buffers and char textures for all fonts
    UI::Renderer::cleanUp(); // destroys quadbuffer and UBO 1
    SoundManager::cleanUp(); // destroys sound buffers
    ResourceLoader::cleanUp(); // destroys textures shaders and models(buffers)


    glfwMakeContextCurrent(nullptr);
    glfwTerminate();
    return 0;
}