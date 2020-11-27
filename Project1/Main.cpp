#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>

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
#include "UI/Panes/Grid.h"
#include "UI/Elements/ImageBox.h"
#include "Physics/Engine.h"
#include "Physics/Collision/Broadphase/NSquared.h"
#include "Physics/Collision/Narrowphase/GJK.h"
#include "Physics/Constraints.h"

#include "MainWindow.h"

#define PBRen 1

Component::Camera cam;
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
        cam.setPos(cam.getForward() * 0.1f + cam.getForward());
    }
}

int main() {
    // glm::vec3 a = { 1, 0, 0 };
    // glm::vec3 b = { 0, 1, 0 };
    // // glm::vec3 c = { 0, 0, 1 };
    // glm::vec3 c = glm::cross(a, b);
    // std::cout << glm::to_string(c) << std::endl; // should be c
    // std::cout << glm::to_string(glm::cross(c, b)) << std::endl; // should be -b
    // return 0;
    


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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glViewport(0, 0, 800, 600);
    glewInit();
    
    cam.setPos({ 0, 0, 0 }); // 200, 700
    
    ResourceLoader::createShader("Basics/DefaultShader");
    ResourceLoader::createShader("Basics/ShadowShader");
    ResourceLoader::createShader("Basics/PBRShader");
    ResourceLoader::createShader("Basics/UIShader");
    ResourceLoader::createShader("Basics/TextShader");
    // cube1
    Primative::Mesh* cubeMesh = new Primative::Mesh();
    Primative::Vertex v1({ -0.5, -0.5, -0.5 }, { 0, 0 });
    Primative::Vertex v2({ 0.5, -0.5, -0.5 }, { 1, 0 });
    Primative::Vertex v3({ 0.5, 0.5, -0.5 }, { 1, 1 });
    Primative::Vertex v4({ -0.5, 0.5, -0.5 }, { 0, 1 });
    Primative::Vertex v5({ -0.5, -0.5, 0.5 }, { 0, 0 });
    Primative::Vertex v6({ 0.5, -0.5, 0.5 }, { 1, 0 });
    Primative::Vertex v7({ 0.5, 0.5, 0.5 }, { 1, 1 });
    Primative::Vertex v8({ -0.5, 0.5, 0.5 }, { 0, 1 });
    
    cubeMesh->verts.push_back(v1);
    cubeMesh->verts.push_back(v2);
    cubeMesh->verts.push_back(v3);
    cubeMesh->verts.push_back(v4);
    cubeMesh->verts.push_back(v5);
    cubeMesh->verts.push_back(v6);
    cubeMesh->verts.push_back(v7);
    cubeMesh->verts.push_back(v8);
    
    cubeMesh->indices = {
        0, 1, 3, 3, 1, 2,
        1, 5, 2, 2, 5, 6,
        5, 4, 6, 6, 4, 7,
        4, 0, 7, 7, 0, 3,
        3, 2, 7, 7, 2, 6,
        4, 5, 0, 0, 5, 1
    };
    std::vector<Primative::VertexBuffer*> cubeBuffers = ResourceLoader::createModel(cubeMesh, "cube", GL_TRIANGLE_STRIP);
    
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
    std::vector<Primative::VertexBuffer*> pistolBuffers = ResourceLoader::createModel("Basics/Models/pistol.obj");
    // auto t = FileReaders::AssimpWrapper::loadModel("Basics/Models/pistol.obj");
    
    // ResourceLoader::createTexture("Basics/Textures/handgun_C.jpg", TextureType::DiffuseMap, 0); // diff
    ResourceLoader::createTexture("Basics/Textures/handgun_S.jpg", TextureType::SpecularMap, 0); // spec
    ResourceLoader::createTexture("Basics/Textures/handgun_N.jpg", TextureType::NormalMap, 0); // norm
    
    // ResourceLoader::createTexture("Basics/Textures/NazghulGreatsword_BaseColor.png", TextureType::AlbedoMap, 0); // albedo
    ResourceLoader::createTexture("Basics/Textures/NazghulGreatsword_Metallic.png", TextureType::MetalicMap, 0); // metalic
    ResourceLoader::createTexture("Basics/Textures/NazghulGreatsword_NormalOpenGL.png", TextureType::NormalMap, 0); // norm
    ResourceLoader::createTexture("Basics/Textures/NazghulGreatsword_AmbientOcclusion.png", TextureType::AOMap, 0); // ao
    ResourceLoader::createTexture("Basics/Textures/NazghulGreatsword_Roughness.png", TextureType::RoughnessMap, 0); // roughness
    
    auto tjiol = ResourceLoader::createTexture("Basics/Textures/Base_color 1.jpg", TextureType::AlbedoMap, 0); // albedo
    ResourceLoader::createTexture("Basics/Textures/metal.jpg", TextureType::MetalicMap, 0); // metalic
    ResourceLoader::createTexture("Basics/Textures/handgun_N.jpg", TextureType::NormalMap, 0); // norm
    ResourceLoader::createTexture("Basics/Textures/gloss.jpg", TextureType::AOMap, 0); // ao
    ResourceLoader::createTexture("Basics/Textures/rough.jpg", TextureType::RoughnessMap, 0); // roughness
    
    
    // unsigned tex = ResourceLoader::createTexture("Basics/Textures/wood.jpg", TextureType::DiffuseMap);
    
    Render::RenderMesh* r = new Render::RenderMesh();
    r->addBuffers(pistolBuffers);
    Materials::Base* mat = new Materials::PBR({ 1 }, { 3 }, { 2 }, { 5 }, { 4 }); // new Materials::PBR({ 1 }, { 3 }, { 2 }, { 5 }, { 4 });
    if (!PBRen) {
        mat = new Materials::Forward({ 1 }, { 2 }, { 3 }, 32);
    }
    r->setMaterial(mat);
    
    GameObject* gun = new GameObject();
    gun->getTransform()->Position = { 0, 0, 0 };
    gun->addComponet(r);
    
    Render::RenderMesh* cubeR = new Render::RenderMesh();
    cubeR->addBuffers(cubeBuffers);
    Materials::Base* cubeMat = new Materials::PBR({ { 1, 0, 0 } }, { { 1, 0, 0 } }, { { 1, 0, 0 } }, { { 1, 0, 0 } }, { { 1, 0, 0 } });
    cubeR->setMaterial(cubeMat);
    
    GameObject* cube1 = new GameObject();
    cube1->getTransform()->Position = { 0, -0.5, -5 };
    cube1->addComponet(cubeR);
    
    Physics::BoxCollider* collider1 = new Physics::BoxCollider(10, Utils::zero());
    cube1->addComponet(collider1);
    
    Component::RigidBody* rb1 = new Component::RigidBody();
    rb1->kinematic = true;
    cube1->addComponet(rb1);
    
    Physics::Constraint* constraint = new Physics::Constraint();
    cube1->getRigidbody()->addConstriant(constraint);
    
    
    Render::RenderMesh* cubeR2 = new Render::RenderMesh();
    auto model = ResourceLoader::getModel("cube");
    cubeR2->addBuffers(model, GL_TRIANGLE_STRIP);
    Materials::Base* cubeMat2 = new Materials::PBR({ { 0, 100, 0 } }, { { 1, 0, 0 } }, { { 1, 0, 0 } }, { { 1, 0, 0 } }, { { 1, 0, 0 } });
    cubeR2->setMaterial(cubeMat2);
    
    GameObject* cube2 = new GameObject();
    cube2->getTransform()->Position = { 5, 5, -5 };
    // cube2->getTransform()->Rotation = glm::rotate(cube2->getTransform()->Rotation, glm::radians(45.0f), Utils::zAxis());
    cube2->addComponet(cubeR2);
    
    Physics::BoxCollider* collider2 = new Physics::BoxCollider(10, Utils::zero());
    cube2->addComponet(collider2);
    
    Component::RigidBody* rb2 = new Component::RigidBody();
    cube2->addComponet(rb2);
    
    cube2->getRigidbody()->addConstriant(constraint);
    
    
    
    
    Primative::StaticBuffer b("m4, m4, v3, f, m4", 0);
    // view    | matrix 4
    // proj    | matrix 4
    // viewPos | vector 3
    // gamma   | scalar f
    // lspaceM | matrix 4
    
    glm::mat4 projection = glm::perspective(glm::radians(cam.getFOV()), 800.0f / 600.0f, 0.01f, 1000.0f);
    
    b.fill(1, glm::value_ptr(projection));
    
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
    Primative::MSAABuffer* finalQB = new Primative::MSAABuffer({ "col" }, { 800, 600 });
    unsigned sceneTex = finalQB->getTextureId("col0");
    
    GameScene scene;
    scene.addFBO("shadows", frameBuffer);
    scene.addFBO("final", finalQB);
    scene.addPreProcLayer("shadows", ResourceLoader::getShader("ShadowShader"));
    scene.addPreProcLayer("final", ResourceLoader::getShader(PBRen ? "PBRShader" : "DefaultShder"));
    scene.setPostProcShader(ResourceLoader::getShader(PBRen ? "PBRShader" : "DefaultShder")); // PBRShader
    // scene.addObject(gun);
    scene.addObject(cube1);
    scene.addObject(cube2);
    
    // UI //
    UI::TextRenderer font = UI::TextRenderer({ 800, 600 }); // creates arial Font
    UI::TextRenderer::setShader(ResourceLoader::getShader("TextShader"));
    UI::Renderer::init(ResourceLoader::getShader("UIShader"), { 800, 600 });
    // UI //
    
    // SOUNDS //
    SoundManager::init();
    const auto buffer = SoundManager::createBuffer("C:/Users/AGWDW/Desktop/iamtheprotectorofthissystem.wav");
    Component::AudioSource* audio = new Component::AudioSource(
        SoundManager::createSoundSource());
    audio->addBuffer(buffer);
    
    Events::Handler::init(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    
    MainWindow win(sceneTex, 8.f/6.f);
    
    Physics::CollisionDetection::setBroadphase(new Physics::NSquared());
    Physics::CollisionDetection::setNarrowphase(new Physics::GJK());
    
   cube2->getRigidbody()->setVelocities({ -1, 0, 0 }, { 0, 0, 0 });

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        fps = 1.0f / deltaTime;
        lastTime = currentTime;
    
        // std::cout << fps << std::endl; // FPS Count

        b.fill(0, value_ptr(cam.getView()));
        b.fill(2, value_ptr(cam.getPos())); 
    
        glDisable(GL_CULL_FACE);
        scene.preProcess(); // shadows and scene quad
        if(!Events::Handler::getKey(Events::Key::Space, Events::Action::Down)){
            scene.postProcess();// render to screen
        }
        else{
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            win.update(); // events check
            //UI::Renderer::render(&win);
        }
        // cube2->getTransform()->Position.y -= 0.5f * deltaTime;
        cube2->getRigidbody()->applyForce({ 0, -9.81, 0 }, { 0, 0, 0 });
        Physics::Engine::update();
    
        if (Events::Handler::getKey(Events::Key::W, Events::Action::Down)) {
            cam.setPos(cam.getPos() + cam.getForward() * deltaTime);
        }
        if (Events::Handler::getKey(Events::Key::S, Events::Action::Down)) {
            cam.setPos(cam.getPos() - cam.getForward() * deltaTime);
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
    delete constraint;
    
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