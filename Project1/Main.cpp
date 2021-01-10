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
#include "Scene/SkyBox.h"
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
#include "Physics/Collision/Narrowphase/GJK3D.h"
#include "Physics/Collision/Narrowphase/SAT3D.h"
#include "Physics/Resolution/ConstraintsBased.h"
#include "Physics/Resolution/ImpulseBased.h"
#include "Componets/Rigidbody.h"
#include "Physics/ConstraintEngine/Constraints/DistanceConstraint.h"
#include "Physics/ConstraintEngine/ConstraitnsSolver.h"

#include "MainWindow.h"
#include "Context.h"


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
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // _crtBreakAlloc = 6157;
    Context main({ 800, 600 }, false);
    main.init("Engine 2", { GL_DEPTH_TEST, GL_CULL_FACE, GL_MULTISAMPLE, GL_TEXTURE_CUBE_MAP_SEAMLESS });

    cam.setPos({ 0, 0, 0 }); // 200, 700
    
    ResourceLoader::createShader("Basics/DefaultShader");
    ResourceLoader::createShader("Basics/ShadowShader");
    ResourceLoader::createShader("Basics/PBRShader");
    ResourceLoader::createShader("Basics/UIShader");
    ResourceLoader::createShader("Basics/TextShader");
        
    // pyramid
    /*Primative::Mesh* m = DBG_NEW Primative::Mesh();
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
        0, 1, 3, 
        3, 1, 2,
        0, 4, 1,
        0, 4, 3,
        2, 4, 1,
        2, 4, 3
    };*/
    
    std::vector<Primative::VertexBuffer*> sphereBuffer = ResourceLoader::createModel("Basics/Models/sphere.obj");
    std::vector<Primative::VertexBuffer*> cubeBuffer   = ResourceLoader::createModel("Basics/Models/cube.obj");
    std::vector<Primative::VertexBuffer*> bsgModel = ResourceLoader::createModel("Basics/Models/bsg.obj");

 
    const unsigned ba   = ResourceLoader::createTexture("Basics/Textures/rust base.png",      TextureType::AlbedoMap);
    const unsigned r    = ResourceLoader::createTexture("Basics/Textures/rust roughness.png", TextureType::RoughnessMap);
    const unsigned m    = ResourceLoader::createTexture("Basics/Textures/rust metalic.png",   TextureType::MetalicMap);
    const unsigned n    = ResourceLoader::createTexture("Basics/Textures/rust normal.png",    TextureType::NormalMap);
    const unsigned hdr  = ResourceLoader::createCubeMap("Basics/Textures/Galaxy hdr", ".png", 0);
    const unsigned ibl  = ResourceLoader::createCubeMap("Basics/Textures/Galaxy ibl", ".png", 0);
    const unsigned brdf = ResourceLoader::createTexture("Basics/Textures/ibl brdf.png", TextureType::AlbedoMap, 0);

        
    Render::RenderMesh cubeR = Render::RenderMesh();
    cubeR.addBuffers(sphereBuffer);
    Materials::PBR cubeMat1/* = Materials::PBR({ { 1, 0, 0 } }, { { 1, 0, 0 } }, { { 0, 0, 0 } }, { { 0.15, 0, 0 } }, { { 0, 0, 0 } })*/;
#define MI Materials::MatItem
    cubeMat1 = Materials::PBR(MI(ba), MI(n), MI(m), MI(r), MI(Utils::xAxis(0.2)));
    cubeMat1.setHDRmap(hdr);
    cubeMat1.setIBLmap(ibl);
    cubeMat1.setBRDFtex(brdf);
    cubeR.setMaterial(&cubeMat1);
    
    GameObject cube1 = GameObject();
    cube1.getTransform()->Position = { 0, 0, -5 };
    cube1.addComponet(&cubeR);
    
    // Physics::BoxColliderSAT collider1 = Physics::BoxColliderSAT(10);
    // cube1->addComponet(&collider1);
    // 
    // Component::RigidBody rb1 = Component::RigidBody();
    // rb1.isKinimatic = true;
    // rb1.hasGravity = false;
    // cube1->addComponet(&rb1);
    
    
    Render::RenderMesh cubeR2 = Render::RenderMesh();
    auto model = ResourceLoader::getModel("sphere.obj");
    cubeR2.addBuffers(model);
    Materials::PBR cubeMat2 = Materials::PBR({ { 0, 1, 1 } }, { { 1, 0, 0 } }, { { 0.5, 0, 0 } }, { { 0.2, 0, 0 } }, { { 0.5, 0, 0 } });
    cubeR2.setMaterial(&cubeMat2);
    
    GameObject cube2 = GameObject();
    cube2.getTransform()->Position = { 0, 1, -5 };
    cube2.addComponet(&cubeR2);
    
    //Physics::BoxColliderSAT collider2 = Physics::BoxColliderSAT(10);
    //cube2->addComponet(&collider2);
    //
    //Component::RigidBody rb2 = Component::RigidBody();
    //cube2->addComponet(&rb2);
    //rb2.hasGravity = false;


    Render::RenderMesh bsgMesh = Render::RenderMesh();
    bsgMesh.addBuffers(bsgModel);
    Materials::PBR bsgMaterial = Materials::PBR({ { 1, 1, 0 } }, { { 1, 0, 0 } }, { { 1, 0, 0 } }, { { 1, 0, 0 } }, { { 1, 0, 0 } });
    bsgMesh.setMaterial(&bsgMaterial);

    GameObject bsgObject = GameObject();
    bsgObject.getTransform()->Position = { 0, 0, -5 };
    bsgObject.getTransform()->Scale *= 0.5f;
    bsgObject.addComponet(&bsgMesh);

    // std::cout << glm::to_string(s1) << " : " << glm::to_string(s2) << std::endl;
    
    
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
    
    glm::mat4 lightView = glm::lookAt(
        glm::vec3(10, 10 ,10),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    
    b.fill(4, value_ptr(lightSpaceMatrix));
    
    // Primative::FrameBuffer* frameBuffer = DBG_NEW Primative::FrameBuffer({ "depth" }, { 800, 600 });
    Primative::MSAABuffer* finalQB = DBG_NEW Primative::MSAABuffer({ "col" }, { 800, 600 });

    GameScene scene;
    // scene.addFBO("shadows", frameBuffer);
    scene.addFBO("final", finalQB);
    // scene.addPreProcLayer("shadows", ResourceLoader::getShader("ShadowShader"));
    scene.addPreProcLayer("final", ResourceLoader::getShader(PBRen ? "PBRShader" : "DefaultShder"));
    scene.setPostProcShader(ResourceLoader::getShader(PBRen ? "PBRShader" : "DefaultShder")); // PBRShader
    // scene.addObject(gun);
    scene.addObject(&cube1);
    scene.addObject(&cube2);
    scene.addObject(&bsgObject);

    scene.setBG({ 1, 1, 1 });

    // SKYBOX //
    ResourceLoader::createCubeMap("Basics/Textures/Galaxy", ".png", 0);
    SkyBox sb = SkyBox("Galaxy.cm");
    //scene.setSkyBox(&sb);
    // SKYBOX //
    

    // UI //
    UI::TextRenderer font = UI::TextRenderer({ 800, 600 }); // creates arial Font
    UI::TextRenderer::setShader(ResourceLoader::getShader("TextShader"));
    UI::Renderer::init(ResourceLoader::getShader("UIShader"), { 800, 600 });

    UI::TextBlock tb = UI::TextBlock();
    tb.setText("FPS: 1000");
    tb.setForgroundColor({ 1, 1, 1 });
    tb.setPos({ 115, 575 });


    // SOUNDS //
   //SoundManager::init();
   //const auto buffer = SoundManager::createBuffer("C:/Users/AGWDW/Desktop/iamtheprotectorofthissystem.wav");
   //Component::AudioSource* audio = DBG_NEW Component::AudioSource(
   //    SoundManager::createSoundSource());
   //audio->addBuffer(buffer);
    // SOUNDS //
    
    Events::Handler::init(main.getWindow());
    glfwSetCursorPosCallback(main.getWindow(), mouse_callback);
        
    Physics::CollisionDetection::setBroadphase<Physics::NSquared>();
    Physics::CollisionDetection::setNarrowphase< Physics::SAT3D>();
    Physics::Engine::setResponse<Physics::ImpulseBased>();

    // Physics::Constraints::ConstraintsSolver::addConstraint<Physics::Constraints::DistanceConstraint>(rb1, rb2, Utils::fill(0.5f), Utils::fill(-0.5f), 1.0f);
    // Physics::Constraints::ConstraintsSolver::addConstraint(DBG_NEW Physics::Constraints::DistanceConstraint(rb1, rb2, Utils::fill(0), Utils::fill(0), 1.5f));

    // cube2->getTransform()->Position = { 0.5, 5, -5 };
    // cube2->getRigidbody()->hasGravity = true;
    //cube2->getRigidbody()->velocityAdder({ 1, -1, 0 });
    while (!main.shouldClose())
    {
        // FPS--------------------------------------------------------------------------------------------------------------------------------------------------

        tb.setText("FPS: " + std::to_string(main.getTime().avgFPS));
        const float w = tb.getWidth() / 2.0f;
        const glm::vec2& p = tb.getPos();
        float diff = p.x - w; // left edge
        diff = 5 - diff;
        tb.setPos({ p.x + diff, p.y });

        // UPDATES----------------------------------------------------------------------------------------------------------------------------------------------
        scene.updateScene();

        // RENDERING--------------------------------------------------------------------------------------------------------------------------------------------
        main.disable(GL_CULL_FACE);
        b.fill(0, value_ptr(cam.getView()));
        b.fill(2, value_ptr(cam.getPos())); 
    
        scene.preProcess(); // shadows and scene quad
        scene.postProcess();// render to screen
        UI::Renderer::render(&tb);

        // PHYSICS-----------------------------------------------------------------------------------------------------------------------------------------------

        // cube2->getTransform()->Position.x -= 0.01;
        Physics::Engine::update();

    
        // EVENTS-----------------------------------------------------------------------------------------------------------------------------------------------
        float speed = 1;
        if(Events::Handler::getCursor(Events::Cursor::Middle, Events::Action::Down)) {
            speed = 2;
        }
        if (Events::Handler::getKey(Events::Key::W, Events::Action::Down)) {
            cam.setPos(cam.getPos() + cam.getForward() * glm::vec3(1, 0, 1) * main.getTime().deltaTime * speed);
        }
        if (Events::Handler::getKey(Events::Key::S, Events::Action::Down)) {
            cam.setPos(cam.getPos() - cam.getForward() * glm::vec3(1, 0, 1) * main.getTime().deltaTime * speed);
        }
        if (Events::Handler::getKey(Events::Key::A, Events::Action::Down)) {
            cam.setPos(cam.getPos() - cam.getRight() * main.getTime().deltaTime * speed);
        }
        if (Events::Handler::getKey(Events::Key::D, Events::Action::Down)) {
            cam.setPos(cam.getPos() + cam.getRight() * main.getTime().deltaTime * speed);
        }
        if (Events::Handler::getKey(Events::Key::Space, Events::Action::Down)) {
            cam.setPos(cam.getPos() + Utils::yAxis() * main.getTime().deltaTime * speed);
        }
        if (Events::Handler::getKey(Events::Key::L_Shift, Events::Action::Down)) {
            cam.setPos(cam.getPos() - Utils::yAxis() * main.getTime().deltaTime * speed);
        }


        if (Events::Handler::getKey(Events::Key::Escape, Events::Action::Down)) {
            break;
        }
        // COLOR BUFFERS----------------------------------------------------------------------------------------------------------------------------------------
        // sound->update();

        // glfwSwapInterval(1); // V-SYNC
        //glfwSwapBuffers(window);
        main.update();

        Events::Handler::pollEvents();
    }
    Render::Shading::Manager::cleanUp(); // deactivats shdaer
    Physics::Engine::cleanUp();
    
    //audio->cleanUp();
    //delete audio;
    //audio = nullptr;

    // delete constraint;
    
    scene.cleanUp(); // removes and destrys all componets and fbos (destroysing comonets doesnt destry buffers(except audio source))
    
    b.cleanUp(); // destroys UBO 0
    UI::TextRenderer::cleanUpStatic(); // destroys char buffers and char textures for all fonts
    UI::Renderer::cleanUp(); // destroys quadbuffer and UBO 1
    SoundManager::cleanUp(); // destroys sound buffers
    ResourceLoader::cleanUp(); // destroys textures shaders and models(buffers)
    
    
    main.cleanUp();
    return 0;
}