#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>
#include <gtx/matrix_decompose.hpp>


#include "Rendering/Rendering.h"
#include "Rendering/Shading/Manager.h"
#include "SoundManager.h"
#include "Utils/ResourceLoader.h"
#include "GameObject/GameObject.h"
#include "Scene/GameScene.h"
#include "Scene/SkyBox.h"
#include "Utils/AssimpWrapper.h"
#include "UI/UIRenderer.h"
#include "EventSystem/Handler.h"
#include "UI/TextRenderer.h"
#include "UI/Elements/TextField.h"
#include "Componets/AudioSource.h"
#include "Componets/Animated.h"
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
#include "Primatives/Model.h"
#include "Gizmos/GizmoRenderer.h"
#include "Gizmos/GizmoShapes.h"

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

    cam.setPos({ 0, 0, 10 }); // 200, 700

    Utils::Timer timer("Shaders");
    timer.start();
    ResourceLoader::createShader("Resources/DefaultShader");
    ResourceLoader::createShader("Resources/ShadowShader");
    ResourceLoader::createShader("Resources/PBRShader");
    ResourceLoader::createShader("Resources/UIShader");
    ResourceLoader::createShader("Resources/TextShader");
    timer.log();

    timer.reName("Models");
    timer.start();
    const Primative::Model manModel    = ResourceLoader::createModel("Resources/Models/RFA_Model.fbx");
    const Primative::Model cubeBuffer  = ResourceLoader::createModel("Resources/Models/cube.obj"); // needed for the skybox
    timer.log();

    timer.reName("Textures");
    timer.start();
    const unsigned ba   = ResourceLoader::loadTexture("Resources/Textures/rust base.png",      TextureType::AlbedoMap);
    const unsigned r    = ResourceLoader::loadTexture("Resources/Textures/rust roughness.png", TextureType::RoughnessMap);
    const unsigned m    = ResourceLoader::loadTexture("Resources/Textures/rust metalic.png",   TextureType::MetalicMap);
    const unsigned n    = ResourceLoader::loadTexture("Resources/Textures/rust normal.png",    TextureType::NormalMap);
    const unsigned hdr  = ResourceLoader::loadCubeMap("Resources/Textures/Galaxy hdr", ".png", 0);
    const unsigned ibl  = ResourceLoader::loadCubeMap("Resources/Textures/Galaxy ibl", ".png", 0);
    const unsigned brdf = ResourceLoader::loadTexture("Resources/Textures/ibl brdf.png", TextureType::AlbedoMap, 0);
    /*ResourceLoader::loadTextureFile("Resources/Textures/RFATextures/Armour", 
        { TextureType::AlbedoMap,TextureType::RoughnessMap,TextureType::MetalicMap,TextureType::NormalMap, TextureType::CubeMap, TextureType::CubeMap, TextureType::AlbedoMap },
        { 1, 1, 1, 1 , 0, 0, 0 });*/
    // Materials::PBR armourMaterial = ResourceLoader::createPBR("Resources/Textures/RFATextures/Armour",
    //     { TextureType::AlbedoMap, TextureType::AOMap, TextureType::MetalicMap, TextureType::NormalMap, TextureType::RoughnessMap },
    //     { 0, 0, 0, 0, 0 });
    // armourMaterial.setHDRmap(hdr);
    // armourMaterial.setIBLmap(ibl);
    // armourMaterial.setBRDFtex(brdf);
    // printf("armour\n");
    // Materials::PBR clothesMaterial = ResourceLoader::createPBR("Resources/Textures/RFATextures/Clothes",
    //         { TextureType::AlbedoMap, TextureType::AOMap, TextureType::MetalicMap, TextureType::NormalMap, TextureType::RoughnessMap },
    //         { 0, 0, 0, 0, 0 });
    // clothesMaterial.setHDRmap(hdr);
    // clothesMaterial.setIBLmap(ibl);
    // clothesMaterial.setBRDFtex(brdf);
    // printf("cloths\n");
    // Materials::PBR headMaterial = ResourceLoader::createPBR("Resources/Textures/RFATextures/Head",
    //     { TextureType::AlbedoMap, TextureType::AOMap, TextureType::MetalicMap, TextureType::NormalMap, TextureType::RoughnessMap },
    //     { 0, 0, 0, 0, 0 });
    // headMaterial.setHDRmap(hdr);
    // headMaterial.setIBLmap(ibl);
    // headMaterial.setBRDFtex(brdf);
    // printf("head\n");
    // Materials::PBR hairMaterial = ResourceLoader::createPBR("Resources/Textures/RFATextures/Hair",
    //     { TextureType::AlbedoMap, TextureType::AOMap, TextureType::RoughnessMap, TextureType::NormalMap },
    //     { 0, 0, 0, 0, 0 });
    // hairMaterial.setHDRmap(hdr);
    // hairMaterial.setIBLmap(ibl);
    // hairMaterial.setBRDFtex(brdf);
    // printf("hair\n");
    // Materials::PBR weponMaterial = ResourceLoader::createPBR("Resources/Textures/RFATextures/Weapon",
    //     { TextureType::AlbedoMap, TextureType::AOMap, TextureType::MetalicMap, TextureType::NormalMap, TextureType::RoughnessMap },
    //     { 0, 0, 0, 0, 0 });
    // weponMaterial.setHDRmap(hdr);
    // weponMaterial.setIBLmap(ibl);
    // weponMaterial.setBRDFtex(brdf);
    // printf("weapon\n");
    timer.log();

    timer.reName("Object 1");
    timer.start();
    Component::RenderMesh manR1 = Component::RenderMesh();
    manR1.setModel(manModel);
    Materials::PBR manMaterial1/* = Materials::PBR({ { 1, 0, 0 } }, { { 1, 0, 0 } }, { { 0, 0, 0 } }, { { 0.15, 0, 0 } }, { { 0, 0, 0 } })*/;
#define MI Materials::MatItem
    manMaterial1 = Materials::PBR(MI(ba), MI(n), MI(m), MI(r), MI(Utils::xAxis(0.2)));
    manMaterial1.setHDRmap(hdr);
    manMaterial1.setIBLmap(ibl);
    manMaterial1.setBRDFtex(brdf);
    manR1.setMaterial(&manMaterial1);
    // manR1.setMaterialTo(&hairMaterial, "Hair");
    // manR1.setMaterialTo(&clothesMaterial, "Cloth");
    // manR1.setMaterialTo(&clothesMaterial, "Scarf");
    // manR1.setMaterialTo(&armourMaterial, "Armour");
    // manR1.setMaterialTo(&headMaterial, "Head");
    // manR1.setMaterialTo(&weponMaterial, "Sword");
    // manR1.setMaterialTo(&weponMaterial, "Dagger");

    Component::Animated manAnimatedComp = Component::Animated();
    auto anim = ResourceLoader::getAnimation("");
    if(anim)
        manAnimatedComp.addAnimation(anim);
    manAnimatedComp.startAnimation("");
    GameObject manObject = GameObject();
    manObject.getTransform()->Position = { 0, 0, 0 };
    manObject.addComponet(&manR1);
    manObject.addComponet(&manAnimatedComp);
    manObject.getTransform()->Scale *= 0.05;
    timer.stop();

    
    timer.reName("Static Buffer");
    timer.start();
    Primative::StaticBuffer b("m4, m4, v3, f, m4", 0);
    // view    | matrix 4
    // proj    | matrix 4
    // viewPos | vector 3
    // gamma   | scalar f
    // lspaceM | matrix 4
    
    glm::mat4 projection = glm::perspective(glm::radians(cam.getFOV()), 800.0f / 600.0f, 0.01f, 5000.0f);
    
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
    timer.log();
    timer.stop();

    timer.reName("Scene");
    timer.start();
    // Primative::FrameBuffer* frameBuffer = DBG_NEW Primative::FrameBuffer({ "depth" }, { 800, 600 });
    Primative::MSAABuffer* finalQB = DBG_NEW Primative::MSAABuffer({ "col" }, { 800, 600 });

    GameScene scene;
    // scene.addFBO("shadows", frameBuffer);
    scene.addFBO("final", finalQB);
    // scene.addPreProcLayer("shadows", ResourceLoader::getShader("ShadowShader"));
    scene.addPreProcLayer("final", ResourceLoader::getShader(PBRen ? "PBRShader" : "DefaultShder"));
    scene.setPostProcShader(ResourceLoader::getShader(PBRen ? "PBRShader" : "DefaultShder")); // PBRShader
    scene.setContext(&main);

    /*for (GameObject& bone : bones) {
        scene.addObject(&bone);
    }*/
    //scene.addObject(&manObject);

    scene.setBG({ 1, 1, 0 });
    timer.log();

    // SKYBOX //
    timer.reName("Skybox");
    timer.start();
    ResourceLoader::loadCubeMap("Resources/Textures/Galaxy", ".png", 0);
    SkyBox sb = SkyBox("Galaxy.cm");
    scene.setSkyBox(&sb);
    timer.log();
    // SKYBOX //
    

    // UI //
    timer.reName("UI");
    timer.start();
    UI::TextRenderer font = UI::TextRenderer({ 800, 600 }); // creates arial Font
    UI::TextRenderer::setShader(ResourceLoader::getShader("TextShader"));
    UI::UIRenderer::init(ResourceLoader::getShader("UIShader"), { 800, 600 });

    UI::TextBlock tb = UI::TextBlock();
    tb.setText("FPS: 1000");
    tb.setForgroundColor({ 1, 1, 1 });
    tb.setPos({ 115, 575 });
    timer.log();

    // SOUNDS //
   //SoundManager::init();
   //const auto buffer = SoundManager::createBuffer("C:/Users/AGWDW/Desktop/iamtheprotectorofthissystem.wav");
   //Component::AudioSource* audio = DBG_NEW Component::AudioSource(
   //    SoundManager::createSoundSource());
   //audio->addBuffer(buffer);
    // SOUNDS //
    
    Events::Handler::init(main.getWindow());
    glfwSetCursorPosCallback(main.getWindow(), mouse_callback);
    
    timer.reName("Physics");
    timer.start();
    Physics::CollisionDetection::setBroadphase<Physics::NSquared>();
    Physics::CollisionDetection::setNarrowphase< Physics::SAT3D>();
    Physics::Engine::setResponse<Physics::ImpulseBased>();
    timer.log();
    // Physics::Constraints::ConstraintsSolver::addConstraint<Physics::Constraints::DistanceConstraint>(rb1, rb2, Utils::fill(0.5f), Utils::fill(-0.5f), 1.0f);
    // Physics::Constraints::ConstraintsSolver::addConstraint(DBG_NEW Physics::Constraints::DistanceConstraint(rb1, rb2, Utils::fill(0), Utils::fill(0), 1.5f));

    // cube2->getTransform()->Position = { 0.5, 5, -5 };
    // cube2->getRigidbody()->hasGravity = true;
    //cube2->getRigidbody()->velocityAdder({ 1, -1, 0 });

    Gizmos::Sphere gizmoSphere({ 0, 0, 0 }, { 1, 0, 0 });
    gizmoSphere.setThickness(2);
    gizmoSphere.setRadius(2);

    Gizmos::GizmoRenderer::addGizmo(&gizmoSphere);

    float counter = 0;
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

        counter += main.getTime().deltaTime;

        // RENDERING--------------------------------------------------------------------------------------------------------------------------------------------
        main.disable(GL_CULL_FACE);
        b.fill(0, value_ptr(cam.getView()));
        b.fill(2, value_ptr(cam.getPos())); 
    
        scene.preProcess(); // shadows and scene quad
        scene.postProcess();// render to screen
        UI::UIRenderer::render(&tb);
        Gizmos::GizmoRenderer::drawAll();

        // PHYSICS-----------------------------------------------------------------------------------------------------------------------------------------------

        // cube2->getTransform()->Position.x -= 0.01;
        Physics::Engine::update();
    
        // EVENTS-----------------------------------------------------------------------------------------------------------------------------------------------
        float speed = 1;
        if(Events::Handler::getCursor(Events::Cursor::Middle, Events::Action::Down)) {
            speed = 10;
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
        counter += main.getTime().deltaTime;
        if (counter >= 0.2 AND Events::Handler::getKey(Events::Key::Enter, Events::Action::Down)) {
            counter = 0;
            manAnimatedComp.togglePlay();
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
    gizmoSphere.cleanUp();
    //audio->cleanUp();
    //delete audio;
    //audio = nullptr;

    // delete constraint;
    
    scene.cleanUp(); // removes and destrys all componets and fbos (destroysing comonets doesnt destry buffers(except audio source))
    
    b.cleanUp(); // destroys UBO 0
    UI::TextRenderer::cleanUpStatic(); // destroys char buffers and char textures for all fonts
    UI::UIRenderer::cleanUp(); // destroys quadbuffer and UBO 1
    SoundManager::cleanUp(); // destroys sound buffers
    ResourceLoader::cleanUp(); // destroys textures shaders and models(buffers)
    
    
    main.cleanUp();
    return 0;
}