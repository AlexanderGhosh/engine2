#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

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
#include "Rendering/Rendering.h"
#include "Componets/AudioSource.h"
#include "Componets/Animated.h"
#include "Componets/Camera.h"
#include "Componets/AudioReciever.h"
#include "Componets/CharacterController.h"
#include "Componets/Lights/PointLight.h"
#include "Componets/Lights/DirectionalLight.h"
#include "Componets/Lights/ShadowCaster.h"

#include "ParticleSystem/ParticleEmmiter.h"
#include "ParticleSystem/Distributions/ConeDistribution.h"
#include "ParticleSystem/Distributions/DomeDistribution.h"
#include "ParticleSystem/Distributions/SphereDistribution.h"

#include "Physics/Engine.h"
#include "Physics/Collision/Broadphase/NSquared.h"
#include "Physics/Collision/Narrowphase/GJK3D.h"
#include "Physics/Collision/Narrowphase/SAT3D.h"
#include "Physics/Resolution/ConstraintsBased.h"
#include "Physics/Resolution/ImpulseBased.h"
#include "Componets/Rigidbody.h"
#include "Physics/ConstraintEngine/Constraints/DistanceConstraint.h"
#include "Physics/ConstraintEngine/ConstraitnsSolver.h"

#include "Primatives/Materials/PBR.h"
#include "Primatives/Materials/MatItemSingle.h"
#include "Primatives/Materials/MatItemChain.h"
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

#include "Context.h"

constexpr glm::ivec2 SCREEN_DIMENTIONS = glm::ivec2(1280, 720);
long SEED;
int main() {
    SEED = time(0);
    // SEED = 1;
    srand(SEED);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // _crtBreakAlloc = 69450;

    Utils::Timer timer;

    Context main(SCREEN_DIMENTIONS, false);
    main.init("Engine 2", { GL_BLEND, GL_DEPTH_TEST, GL_CULL_FACE, GL_MULTISAMPLE });
    Events::Handler::init(main.getWindow());

    SoundManager::init();
    Primative::Buffers::SoundBuffer* soundDatabuffer = SoundManager::createBuffer("Resources/Sounds/bounce.wav");
    Primative::Buffers::SoundStreamBuffer* soundDatabufferS = SoundManager::createBuffer("Resources/Sounds/iamtheprotectorofthissystem.wav", true);

    timer.start("Shaders");
    Gizmos::GizmoRenderer::init();
    std::vector<std::string> shaders = {
        "PBRShader", "DeferredRendering/TransparentShader", "Shadows/ShadowShader", "UIShader", "TextShader", "DeferredRendering/G Pass/DeferredTerrainMap", "DeferredRendering/G Pass/DeferredTerrainMesh", "PostShader", "ParticleShader", 
        "DeferredRendering/DeferredFinal", "DeferredRendering/G Pass/DeferredOpaque", "GausianBlur"
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
    const Primative::Model cubeBuffer  = ResourceLoader::createModel("Resources/Models/cube.obj"); // needed for the skybox
    const Primative::Model planeBuffer = ResourceLoader::createModel("Resources/Models/plane.dae");
    // const Primative::Model minikitBuffer = ResourceLoader::createModel("Resources/Models/minikit.fbx");
    const Primative::Model orbBuffer = ResourceLoader::createModel("Resources/Models/sphere.obj");
    //const Primative::Model cityBuffer = ResourceLoader::createModel("Resources/Models/city.obj");
    timer.log();

    timer.start("Animations");
    // const auto manAnimations = ResourceLoader::createAnimation("Resources/Models/Animations/RFA_Attack.fbx", manModel.getSkeleton());
    timer.log();

    timer.start("Textures");
    const unsigned wi   = ResourceLoader::loadTexture("Resources/Textures/window.png", TextureType::AlbedoMap, 0);
    const unsigned wiy  = ResourceLoader::loadTexture("Resources/Textures/yellowWindow.png", TextureType::AlbedoMap, 0);
    // const unsigned hm       = ResourceLoader::loadTexture("Resources/Textures/heightmap.png", TextureType::HeightMap, 0);
    const unsigned rainDrop = ResourceLoader::loadTexture("Resources/Textures/raindrop.png", TextureType::AlbedoMap, 0);
    const unsigned dirt     = ResourceLoader::loadTexture("Resources/Textures/dirt.png", TextureType::AlbedoMap, 0);
    //const unsigned cityTex  = ResourceLoader::loadTexture("Resources/Textures/City Tex.png", TextureType::AlbedoMap, 0);

    auto grassMaterialInfo = ResourceLoader::createPBRInfo("Resources/Textures/Grass", { TextureType::AlbedoMap, TextureType::AOMap, TextureType::MetalicMap, TextureType::NormalMap, TextureType::RoughnessMap }, { 0, 0, 0, 0, 0 });
    auto& grassMaterial = ResourceLoader::createPBR(grassMaterialInfo);
    grassMaterial.setRepeatValue(10);

    auto waterMaterialInfo = ResourceLoader::createPBRInfo("Resources/Textures/Water", { TextureType::AlbedoMap, TextureType::RoughnessMap, TextureType::NormalMap, TextureType::AOMap, TextureType::MetalicMap }, { 0, 0, 0, 0, 0 });
    auto& waterMaterial = ResourceLoader::createPBR(waterMaterialInfo);
    Materials::MatItemSingle<glm::vec4> waterAlbedo({ 0, 0, 1, 1 });
    Materials::MatItemSingle<float> waterMetalic(0.5f);
    Materials::MatItemSingle<float> waterRoughness(0.5f);
    Materials::MatItemSingle<float> waterAO(0.5f);

    waterMaterial.setAlbedo(&waterAlbedo);
    waterMaterial.setMetalic(&waterMetalic);
    waterMaterial.setRoughness(&waterRoughness);
    waterMaterial.setAO(&waterAO);
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
#define MI1 Materials::MatItemSingle<float>
#define MI3 Materials::MatItemSingle<glm::vec3>
#define MI4 Materials::MatItemSingle<glm::vec4>
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


    /*std::vector<MI1> mi1s = {
        MI1(0.5f), MI1(0.0f)
    };
    std::vector<MI3> mi3s = {
        MI3({1.0f, 1.0f, 1.0f})
    };
    std::vector<MI4> mi4s = {
        MI4({ 1, 1, 1, 1 }), 
        MI4({ 0.5, 0.5, 0.5, 1 }), 
        MI4({ 0, 0, 0, 1 }),
        MI4({ 0, 0.75, 0, 1 }),
        MI4({ 0.75, 0, 0, 1 }),
    };


    GameObject minikit({ 0, 0, 0 }, { 0.5, 0.5, 0.5 });
    Component::RenderMesh minikitMesh;
    minikitMesh.setModel(minikitBuffer);

    Materials::MatItemChain<glm::vec4> whiteChain = Materials::MatItemChain<glm::vec4>({glm::vec4(1), glm::vec4(1, 0, 1, 1) }, {0, 0}, {0, 0}, {1, 1});

    Materials::PBR minikitMatWhite(&whiteChain, &mi3s[0], &mi1s[0], &mi1s[0], &mi1s[1]);
    Materials::PBR minikitMatGray (&mi4s[1], &mi3s[0], &mi1s[0], &mi1s[0], &mi1s[1]);
    Materials::PBR minikitMatBlack(&mi4s[2], &mi3s[0], &mi1s[0], &mi1s[0], &mi1s[1]);
    Materials::PBR minikitMatGreen(&mi4s[3], &mi3s[0], &mi1s[0], &mi1s[0], &mi1s[1]);
    Materials::PBR minikitMatRed  (&mi4s[4], &mi3s[0], &mi1s[0], &mi1s[0], &mi1s[1]);

    minikitMesh.setMaterialTo(&minikitMatWhite, "white");
    minikitMesh.setMaterialTo(&minikitMatGray,  "gray");
    minikitMesh.setMaterialTo(&minikitMatBlack, "black");
    minikitMesh.setMaterialTo(&minikitMatGreen, "green");
    minikitMesh.setMaterialTo(&minikitMatRed,   "red");

    minikit.addComponet(&minikitMesh);
    SpinScript spinScript({ -1, 1, 0 }, 0.125);
    minikit.addComponet(&spinScript);
    HoverScript hoverScript(1, 0.25);
    minikit.addComponet(&hoverScript);
    Component::AudioSource audio(SoundManager::createSoundSource());
    // audio.addBuffer(dynamic_cast<Primative::Buffers::SoundBuffer*>(soundDatabufferS));
    audio.addBuffer(soundDatabuffer);
    minikit.addComponet(&audio);
    //audio.play();
    SoundControllerScript scs;
    minikit.addComponet(&scs);
    Component::ParticleEmmiter emitter(200, 2, { 0, 0, 0 }, { 1.5, 2, 1.5 });
    //emitter.loop();
    minikit.addComponet(&emitter);
    emitter.setTexture(Primative::TextureChain({ rainDrop, hm }, { 1, 1 }));
    RainMakerScript rain;
    minikit.addComponet(&rain);*/


    GameObject orb(glm::vec3(0, 1.5, 0));
    Component::RenderMesh orbMesh = Component::RenderMesh();
    orbMesh.setModel(orbBuffer);
    orbMesh.setMaterial(&waterMaterial);
    orb.addComponet(&orbMesh);

    /*Particles::DomeDistribution distribution = Particles::DomeDistribution(1.0f);
    Component::ParticleEmmiter emmiter = Component::ParticleEmmiter(10, 10, true, 0.25);
    Materials::MatItemSingle<glm::vec4> particleAlbedo(rainDrop);
    emmiter.setAlbedo(&particleAlbedo);
    emmiter.setShape(&distribution);
    orb.addComponet(&emmiter);*/

    timer.log();

    timer.start("Terrain"); 
    /*MI3 lowestColour(dirt);
    MI3 middleColour(dirt);
    MI3 highestColour(grass, 10.0f);

    Materials::MatItemSingle<glm::vec4> landAlbedo(grass, 20.0f);
    Materials::MatItemSingle<glm::vec3> landNormal(glm::vec3(0, 1, 0));
    Materials::MatItemSingle<float> landMetalic(0.4f);
    Materials::MatItemSingle<float> landRoughness(0.7f);
    Materials::MatItemSingle<float> landAO(0.5f);

    Materials::PBR landMaterial = Materials::PBR(&landAlbedo, &landNormal, &landMetalic, &landRoughness, &landAO);*/

    const int landSize = 2;
    const float scale = 100;
    const int landRes = 100;
    std::vector<Terrain> allLand;
    allLand.reserve(landSize * landSize);
    for (char i = 0; i < landSize; i++) {
        for (char j = 0; j < landSize; j++) {

            Terrain land(landRes);
            land.renderWireframe = false;

            land.setPosition(glm::vec3(i * scale, -0.5f, j * scale));
            land.setScale({ scale, 10, scale });
            // land.setNoiseBuffer(Utils::NoiseGeneration::getMap(glm::vec3(i, j, 0), landRes + 1, { 1, 0.5, 0.1 }, { 1, 2, 3 }));
            land.useTextureMap(false);
            land.setMaterial(&grassMaterial);
            /*land.setLowestTexture(&lowestColour);
            land.setMiddleTexture(&middleColour);
            land.setHighestTexture(&highestColour);*/

            allLand.push_back(land);
        }
    }
    timer.log();



    timer.start("Player");
    GameObject player = GameObject(glm::vec3(0, 0, 5));
    Component::Camera playerCamera = Component::Camera();
    player.addComponet(&playerCamera);
    Component::CharacterController cc;
    player.addComponet(&cc);
    PlayerControler playerScript;
    playerScript.ground = allLand;
    player.addComponet(&playerScript);
    DebugScreen debugScript;
    player.addComponet(&debugScript);
    Component::AudioReciever recieverComp;
    player.addComponet(&recieverComp);


    GameObject lightSource(glm::vec3(0, 1.5, -2.5));
    Component::PointLight light(glm::vec3(1), 100.0f);
    lightSource.addComponet(&light);

    GameObject lightSource2(glm::vec3(0, 3.75, 0));
    Component::DirectionalLight light2 = Component::DirectionalLight({ 1, 1, 1 }, { 1, 1, 1 }, 1);
    lightSource2.addComponet(&light2);
    Component::ShadowCaster shadowCaster = Component::ShadowCaster(&light2, 15);
    lightSource2.addComponet(&shadowCaster);

   /* Materials::MatItemSingle<glm::vec4> cityAlbedo({ 0, 1, 0, 1 });
    Materials::MatItemSingle<glm::vec3> cityNorm({ 0, 1, 0 });
    Materials::MatItemSingle<float> cityMetalic(0.5f);
    Materials::MatItemSingle<float> cityRoughness(0.5f);
    Materials::MatItemSingle<float> cityAO(0.5f);
    Materials::PBR cityMaterial(&cityAlbedo, &cityNorm, &cityMetalic, &cityRoughness, &cityAO);

    GameObject city(glm::vec3(0, 10, 0));
    Component::RenderMesh cityMesh;
    cityMesh.setModel(cityBuffer);
    cityMesh.setMaterial(&cityMaterial);
    city.addComponet(&cityMesh);*/




    Materials::MatItemSingle<glm::vec4> windowAlbedo(wi);
    Materials::MatItemSingle<glm::vec3> windowNormal(glm::vec3(0, 1, 0));
    Materials::MatItemSingle<float> windowMetalic(0.5f);
    Materials::MatItemSingle<float> windowRoughness(0.0f);
    Materials::MatItemSingle<float> windowAO(0.5f);

    Materials::PBR windowMat1(&windowAlbedo, &windowNormal, &windowMetalic, &windowRoughness, &windowAO);

    Materials::MatItemSingle<glm::vec4> windowAlbedo2(wiy);

    Materials::PBR windowMat2(&windowAlbedo2, &windowNormal, &windowMetalic, &windowRoughness, &windowAO);

    GameObject window1(glm::vec3(0, 3.5, -3));
    Component::RenderMesh windowMesh1;
    windowMesh1.setModel(planeBuffer);
    windowMesh1.setMaterial(&windowMat1);
    windowMesh1.setTransparent(true);
    window1.addComponet(&windowMesh1);

    /*GameObject window2(glm::vec3(0.5, 3.5, -4));
    Component::RenderMesh windowMesh2;
    windowMesh2.setModel(planeBuffer);
    windowMesh2.setMaterial(&windowMat2);
    windowMesh2.setTransparent(true);
    window2.addComponet(&windowMesh2);*/

    timer.log();


    timer.start("Scene");
    //Primative::MSAABuffer* finalQB = DBG_NEW Primative::MSAABuffer({ "col" }, { 800, 600 });

    GameScene scene;
    scene.setContext(&main);
    scene.setMainCamera(&playerCamera);
    scene.setBG({ 1, 0.5, 0.25 });
    scene.initalize();

    scene.addObject(&player);
    scene.addObject(&lightSource);
    scene.addObject(&lightSource2);
    scene.addObject(&window1);
    //scene.addObject(&city);

    scene.addObject(&orb);
    for (Terrain& land : allLand) {
        scene.addTerrain(&land);
    }
    // scene.addTerrain(&land);

    scene.setShadowCaster(&shadowCaster);

    timer.log();

    // SKYBOX //
    timer.start("Skybox");
    ResourceLoader::loadCubeMap("Resources/Textures/DistantMtSB", ".png", 0);
    SkyBox sb = SkyBox("DistantMtSB.cm");
    scene.setSkyBox(&sb);
    timer.log();
    // SKYBOX //
    
    
    // timer.reName("Physics");
    // timer.start();
    // Physics::CollisionDetection::setBroadphase<Physics::NSquared>();
    // Physics::CollisionDetection::setNarrowphase< Physics::SAT3D>();
    // Physics::Engine::setResponse<Physics::ImpulseBased>();
    // timer.log();
    // Physics::Constraints::ConstraintsSolver::addConstraint<Physics::Constraints::DistanceConstraint>(rb1, rb2, Utils::fill(0.5f), Utils::fill(-0.5f), 1.0f);
    // Physics::Constraints::ConstraintsSolver::addConstraint(DBG_NEW Physics::Constraints::DistanceConstraint(rb1, rb2, Utils::fill(0), Utils::fill(0), 1.5f));

    // cube2->getTransform()->Position = { 0.5, 5, -5 };
    // cube2->getRigidbody()->hasGravity = true;
    //cube2->getRigidbody()->velocityAdder({ 1, -1, 0 });

    Gizmos::Sphere gizmo1 = Gizmos::Sphere({ 0, 4, 0 }, { 1, 0, 0 });
    gizmo1.setRadius(0.25);
    gizmo1.setThickness(2);
    Gizmos::GizmoRenderer::addGizmo(&gizmo1);

    Utils::log("Started Loop");
    scene.gameLoop();

    /*while (!main.shouldClose())
    {

        // PHYSICS-----------------------------------------------------------------------------------------------------------------------------------------------

        // cube2->getTransform()->Position.x -= 0.01;
        Physics::Engine::update();
    }*/
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