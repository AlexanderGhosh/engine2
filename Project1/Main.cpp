#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>
#include <gtx/matrix_decompose.hpp>

#include "SoundManager.h"
#include "Utils/ResourceLoader.h"
#include "GameObject/GameObject.h"
#include "Scene/GameScene.h"
#include "Scene/SkyBox.h"
#include "UI/UIRenderer.h"
#include "EventSystem/Handler.h"
#include "UI/TextRenderer.h"
#include "Componets/AudioSource.h"
#include "Componets/Animated.h"
#include "Componets/Camera.h"
#include "Rendering/Rendering.h"
#include "Componets/AudioReciever.h"
#include "ParticleSystem/ParticleEmmiter.h"

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
#include "GameObject/Terrain.h"
#include "Primatives/Buffers/FrameBuffer.h"
#include "Primatives/Buffers/UniformBuffer.h"
#include "Primatives/Buffers/SoundBuffer.h"
#include "Primatives/Buffers/SoundStreamBuffer.h"
#include "Utils/NoiseGeneration.h"
#include "Scripts/PlayerControler.h"
#include "Scripts/DebugScreen.h"
#include "Scripts/SpinScript.h"
#include "Scripts/HoverScript.h"
#include "Scripts/SoundControllerScript.h"
#include "Scripts/RainMakerScript.h"

#include "Context.h"

constexpr glm::ivec2 SCREEN_DIMENTIONS = glm::ivec2(1280, 720);

int main() {
    srand(time(0));
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // _crtBreakAlloc = 69450;

    Utils::Timer timer;

    Context main = Context(SCREEN_DIMENTIONS, true, 60);
    main.init("Engine 2", { GL_BLEND, GL_DEPTH_TEST, GL_CULL_FACE, GL_MULTISAMPLE });
    Events::Handler::init(main.getWindow());

    SoundManager::init();
    Primative::Buffers::SoundBuffer* soundDatabuffer = SoundManager::createBuffer("Resources/Sounds/bounce.wav");
    Primative::Buffers::SoundStreamBuffer* soundDatabufferS = SoundManager::createBuffer("Resources/Sounds/iamtheprotectorofthissystem.wav", true); // 407640__drotzruhn__countdown-30-seconds

    timer.start("Shaders");
    Gizmos::GizmoRenderer::init();
    std::vector<std::string> shaders = {
        "PBRShader", "TransparentShader", "ShadowShader", "UIShader", "TextShader", "TerrainShaderHeightMap", "TerrainShaderMesh", "PostShader", "ParticleShader"
    };
    ResourceLoader::createShaders(shaders);
    timer.log();



    // UI //
    timer.start("UI");
    UI::TextRenderer font = UI::TextRenderer(SCREEN_DIMENTIONS, "arial", 25); // creates arial Font
    UI::TextRenderer::setShader(ResourceLoader::getShader("TextShader"));
    UI::UIRenderer::init(ResourceLoader::getShader("UIShader"), SCREEN_DIMENTIONS);
    timer.log();
    // UI //

    timer.start("Models");
    // const Primative::Model manModel    = ResourceLoader::createModel("Resources/Models/RFA_Model.fbx"); // RFA_Model
    const Primative::Model cubeBuffer  = ResourceLoader::createModel("Resources/Models/cube.dae"); // needed for the skybox
    const Primative::Model planeBuffer = ResourceLoader::createModel("Resources/Models/plane.dae");
    const Primative::Model sphereBuffer = ResourceLoader::createModel("Resources/Models/sphere.dae");
    // const Primative::Model minikitBuffer = ResourceLoader::createModel("Resources/Models/minikit.fbx");
    timer.log();

    timer.start("Animations");
    // const auto manAnimations = ResourceLoader::createAnimation("Resources/Models/Animations/RFA_Attack.fbx", manModel.getSkeleton());
    timer.log();

    timer.start("Textures");
    // const unsigned wi   = ResourceLoader::loadTexture("Resources/Textures/window.png", TextureType::AlbedoMap, 0);
    // const unsigned wiy  = ResourceLoader::loadTexture("Resources/Textures/yellowWindow.png", TextureType::AlbedoMap, 0);
    // const unsigned hdr  = ResourceLoader::loadCubeMap("Resources/Textures/Galaxy hdr", ".png", 0);
    // const unsigned ibl  = ResourceLoader::loadCubeMap("Resources/Textures/Galaxy ibl", ".png", 0);
    // const unsigned brdf = ResourceLoader::loadTexture("Resources/Textures/ibl brdf.png", TextureType::AlbedoMap, 0);
    // const unsigned hm   = ResourceLoader::loadTexture("Resources/Textures/heightmap.png", TextureType::HeightMap, 0);
    // const unsigned rainDrop = ResourceLoader::loadTexture("Resources/Textures/raindrop.png", TextureType::AlbedoMap, 0);

   //Materials::PBR armourMaterial = ResourceLoader::createPBR("Resources/Textures/RFATextures/Armour",
   //    { TextureType::AlbedoMap, TextureType::AOMap, TextureType::MetalicMap, TextureType::NormalMap, TextureType::RoughnessMap },
   //    { 0, 0, 0, 0, 0 });
   //armourMaterial.setHDRmap(hdr);
   //armourMaterial.setIBLmap(ibl);
   //armourMaterial.setBRDFtex(brdf);
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

    timer.start("Objects");
    //Component::RenderMesh manR1 = Component::RenderMesh();
    //manR1.setModel(manModel);
    //Materials::PBR manMaterial1/* = Materials::PBR({ { 1, 0, 0 } }, { { 1, 0, 0 } }, { { 0, 0, 0 } }, { { 0.15, 0, 0 } }, { { 0, 0, 0 } })*/;
#define MI Materials::MatItem
   // manR1.setMaterial(&armourMaterial);
    //manR1.setMaterialTo(&hairMaterial, "Hair");
    //manR1.setMaterialTo(&clothesMaterial, "Cloth");
    //manR1.setMaterialTo(&clothesMaterial, "Scarf");
    //manR1.setMaterialTo(&armourMaterial, "Armour");
    //manR1.setMaterialTo(&headMaterial, "Head");
    //manR1.setMaterialTo(&weponMaterial, "Sword");
    //manR1.setMaterialTo(&weponMaterial, "Dagger");

    // Component::Animated manAnimatedComp = Component::Animated();
    // const std::string AnimationLoaded = "Rig|man_run_in_place";
    // auto anim = ResourceLoader::getAnimation(AnimationLoaded); // RFA_Attack
    // if(anim)
    //     manAnimatedComp.addAnimation(anim);
    // GameObject manObject = GameObject();
    // manObject.getTransform()->Position = { 0, -1, 0 };
    // manObject.addComponet(&manR1);
    // manObject.addComponet(&manAnimatedComp);
    // manObject.getTransform()->Scale *= 0.0125;
    // 
    // 
    // GameObject redWindow({ 0, 0, 3 }, { 1, 1, 1 });
    // Component::RenderMesh plane;
    // plane.setTransparent(true);
    // plane.setModel(planeBuffer);
    // Materials::PBR planeMat(MI(wi), MI({ 0.5, 0, 0 }), MI({ 0.5, 0, 0 }), MI({ 0.5, 0, 0 }), MI(Utils::xAxis(0.2)));
    // plane.setMaterial(&planeMat);
    // redWindow.addComponet(&plane);
    // 
    // GameObject yellowWindow({ 0.5, 0, 2 }, { 1, 1, 1 });
    // Component::RenderMesh plane_y;
    // plane_y.setTransparent(true);
    // plane_y.setModel(planeBuffer);
    // Materials::PBR planeMat_y(MI(wiy), MI({ 0.5, 0, 0 }), MI({ 0.5, 0, 0 }), MI({ 0.5, 0, 0 }), MI(Utils::xAxis(0.2)));
    // plane_y.setMaterial(&planeMat_y);
    // yellowWindow.addComponet(&plane_y);

    // GameObject minikit({ 0, 0, 0 }, { 0.5, 0.5, 0.5 });
    // Component::RenderMesh minikitMesh;
    // minikitMesh.setModel(minikitBuffer);
    // Materials::PBR minikitMatWhite(MI({ 1, 1, 1 }),       MI({ 1, 1, 1 }), MI({ 0.5, 0.5, 0.5 }), MI({ 0.5, 0, 0 }), MI({ 0, 0, 0 }));
    // Materials::PBR minikitMatGray( MI({ 0.5, 0.5, 0.5 }), MI({ 1, 1, 1 }), MI({ 0.5, 0.5, 0.5 }), MI({ 0.5, 0, 0 }), MI({ 0, 0, 0 }));
    // Materials::PBR minikitMatBlack(MI({ 0, 0, 0 }),       MI({ 1, 1, 1 }), MI({ 0.5, 0.5, 0.5 }), MI({ 0.5, 0, 0 }), MI({ 0, 0, 0 }));
    // Materials::PBR minikitMatGreen(MI({ 0, 0.75, 0 }),    MI({ 1, 1, 1 }), MI({ 0.5, 0.5, 0.5 }), MI({ 0.5, 0, 0 }), MI({ 0, 0, 0 }));
    // Materials::PBR minikitMatRed(  MI({ 0.75, 0, 0 }),    MI({ 1, 1, 1 }), MI({ 0.5, 0.5, 0.5 }), MI({ 0.5, 0, 0 }), MI({ 0, 0, 0 }));
    // minikitMesh.setMaterialTo(&minikitMatWhite, "white");
    // minikitMesh.setMaterialTo(&minikitMatGray,  "gray");
    // minikitMesh.setMaterialTo(&minikitMatBlack, "black");
    // minikitMesh.setMaterialTo(&minikitMatGreen, "green");
    // minikitMesh.setMaterialTo(&minikitMatRed,   "red");
    // minikit.addComponet(&minikitMesh);
    // SpinScript spinScript({ -1, 1, 0 }, 0.125);
    // minikit.addComponet(&spinScript);
    // HoverScript hoverScript(1, 0.25);
    // minikit.addComponet(&hoverScript);
    // Component::AudioSource audio(SoundManager::createSoundSource());
    // // audio.addBuffer(dynamic_cast<Primative::Buffers::SoundBuffer*>(soundDatabufferS));
    // audio.addBuffer(soundDatabuffer);
    // minikit.addComponet(&audio);
    // //audio.play();
    // SoundControllerScript scs;
    // minikit.addComponet(&scs);
    // Component::ParticleEmmiter emitter(200, 2, { 0, 0, 0 }, { 1.5, 2, 1.5 });
    // //emitter.loop();
    // minikit.addComponet(&emitter);
    // emitter.setTexture(Primative::TextureChain({ rainDrop, hm }, { 1, 1 }));
    // RainMakerScript rain;
    // minikit.addComponet(&rain);

    timer.log();

    // timer.start("Terrain");
    // Terrain land(100);
    // land.getTransform().Position.y = -1;
    // land.getTransform().Scale = { 100, 10 ,100 };
    // land.setHeightMap(hm);
    // land.setNoiseBuffer(Utils::NoiseGeneration::getMap(100, { 1, 0.5, 0.1 }, { 1, 2, 3 }));
    // land.useTextureMap(false);
    // timer.log();

    constexpr glm::vec3 BOX_OFFSET(0, -2, 0);
    // constexpr glm::vec3 BOX_OFFSET(-4, 1, -2);
    
    GameObject topBox = GameObject(glm::vec3(0, 3, 0) + BOX_OFFSET, glm::vec3(0.5));
    //topBox.getTransform()->rotate({ 0, 0, RADIANS(45) });
    Component::RenderMesh topCubeMesh;
    topCubeMesh.setModel(sphereBuffer);
    Materials::PBR topMat = Materials::PBR(MI({ 1, 0, 0 }), MI({ 1, 1, 1 }), MI({ 0.5, 0.5, 0.5 }), MI({ 0.5, 0, 0 }), MI({ 0, 0, 0 }));
    topCubeMesh.setMaterial(&topMat);
    topBox.addComponet(&topCubeMesh);
    Component::RigidBody rbTop;
    rbTop.isKinimatic = true;
    rbTop.hasGravity = true;
    Physics::BoxColliderSAT topCollider = Physics::BoxColliderSAT(10);
    topBox.addComponet(&rbTop);
    topBox.addComponet(&topCollider);

    Physics::Constraints::DistanceConstraint distanceConstraint = Physics::Constraints::DistanceConstraint({ 0, 0, 0 }, { -0.5, 0.5, -0.5 }, 0); // { -0.5, 0.5, -0.5 }
    distanceConstraint.setBodyA(&rbTop);
    topBox.getRigidbody()->constraints.push_back(&distanceConstraint);
    

    GameObject bottomBox = GameObject(glm::vec3(0.5, 2.5, 0.5) + BOX_OFFSET, { 1, 1, 1 }); // glm::vec3(0.5, 2.5, 0.5)
    Component::RenderMesh bottomCubeMesh;
    bottomCubeMesh.setModel(cubeBuffer);
    Materials::PBR bottomMat = Materials::PBR(MI({ 0, 0, 1 }), MI({ 1, 1, 1 }), MI({ 0.5, 0.5, 0.5 }), MI({ 0.5, 0, 0 }), MI({ 0, 0, 0 }));
    bottomCubeMesh.setMaterial(&bottomMat);
    bottomBox.addComponet(&bottomCubeMesh);
    Component::RigidBody rbBottom;
    Physics::BoxColliderSAT bottomCollider = Physics::BoxColliderSAT(10);
    bottomBox.addComponet(&rbBottom);
    bottomBox.addComponet(&bottomCollider);
    rbBottom.isKinimatic = false;
    rbBottom.hasGravity = true;

    distanceConstraint.setBodyB(&rbBottom);

    timer.start("Player");
    GameObject player = GameObject({ 0, 0, 5 }, { 1, 1, 1 });
    Component::Camera playerCamera = Component::Camera();
    player.addComponet(&playerCamera);
    PlayerControler playerScript;
    player.addComponet(&playerScript);
    DebugScreen debugScript;
    debugScript.RedBlock = &topBox;
    debugScript.BlueBlock = &bottomBox;
    player.addComponet(&debugScript);
    Component::AudioReciever recieverComp;
    player.addComponet(&recieverComp);

    timer.log();


    timer.start("Scene");
    //Primative::MSAABuffer* finalQB = DBG_NEW Primative::MSAABuffer({ "col" }, { 800, 600 });

    GameScene scene;
    scene.setMainCamera(&playerCamera);
    scene.setContext(&main);
    scene.setBG({ 0.5, 0.5, 0.5 });
    scene.initalize();

    scene.addObject(&player);
    scene.addObject(&topBox);
    scene.addObject(&bottomBox);

    timer.log();

    // SKYBOX //
    timer.start("Skybox");
    // ResourceLoader::loadCubeMap("Resources/Textures/Galaxy", ".png", 0);
    // SkyBox sb = SkyBox("Galaxy.cm");
    // scene.setSkyBox(&sb);
    timer.log();
    // SKYBOX //
    
    
    timer.reName("Physics");
    timer.start();
    Physics::CollisionDetection::setBroadphase<Physics::NSquared>();
    Physics::CollisionDetection::setNarrowphase< Physics::SAT3D>();
    Physics::Engine::setResponse<Physics::ConstraintsBased>();
    timer.log();
    // Physics::Constraints::ConstraintsSolver::addConstraint<Physics::Constraints::DistanceConstraint>(rb1, rb2, Utils::fill(0.5f), Utils::fill(-0.5f), 1.0f);
    // Physics::Constraints::ConstraintsSolver::addConstraint(DBG_NEW Physics::Constraints::DistanceConstraint(rb1, rb2, Utils::fill(0), Utils::fill(0), 1.5f));
    
    // cube2->getTransform()->Position = { 0.5, 5, -5 };
    // cube2->getRigidbody()->hasGravity = true;
    // cube2->getRigidbody()->velocityAdder({ 1, -1, 0 });

    Gizmos::Sphere gizmo1 = Gizmos::Sphere({ 0, 0, 0 }, { 1, 0, 0 });
    gizmo1.setRadius(0.25);
    gizmo1.setThickness(2);
    Gizmos::GizmoRenderer::addGizmo(&gizmo1);

    scene.gameLoop();

    Physics::Engine::cleanUp();
    gizmo1.cleanUp();
    
    scene.cleanUp(); // removes and destrys all componets and fbos (destroysing comonets doesnt destry buffers(except audio source))
    
    UI::TextRenderer::cleanUpStatic(); // destroys char buffers and char textures for all fonts
    UI::UIRenderer::cleanUp(); // destroys quadbuffer and UBO 1
    SoundManager::cleanUp(); // destroys sound buffers
    ResourceLoader::cleanUp(); // destroys textures shaders and models(buffers)
    Gizmos::GizmoRenderer::cleanUp();
    
    return 0;
}