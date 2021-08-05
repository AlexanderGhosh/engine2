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

#include "GUI/ElementContainers/GUICanvas.h"
#include "GUI/UIElementBase.h"
#include "GUI/GUIConstraint.h"
#include "GUI/Constraints/PixelConstraint.h"
#include "GUI/Constraints/PercentConstraint.h"
#include "GUI/Constraints/SpanConstraint.h"
#include "GUI/Elements/GUISlider.h"
#include "GUI/Elements/GUITextBlock.h"
#include "GUI/Text/Font.h"
#include "GUI/Text/TextRendering.h"

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
        "DeferredRendering/DeferredFinal", "DeferredRendering/G Pass/DeferredOpaque", "GausianBlur", "GUI/BaseShapeShader"
    };
    ResourceLoader::createShaders(shaders);
    timer.log();

    // GUI //

    GUI::Font arail = GUI::Font("arial");
    GUI::TextRendering::addFont(&arail);

    // GUI //

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
    // const Primative::Model orbBuffer = ResourceLoader::createModel("Resources/Models/sphere.obj");
    //const Primative::Model cityBuffer = ResourceLoader::createModel("Resources/Models/city.obj");
    timer.log();

    timer.start("Animations");
    // const auto manAnimations = ResourceLoader::createAnimation("Resources/Models/Animations/RFA_Attack.fbx", manModel.getSkeleton());
    timer.log();

    timer.start("Textures");
    // const unsigned wi   = ResourceLoader::loadTexture("Resources/Textures/window.png", TextureType::AlbedoMap, 0);
    // const unsigned wiy  = ResourceLoader::loadTexture("Resources/Textures/yellowWindow.png", TextureType::AlbedoMap, 0);
    // const unsigned hm       = ResourceLoader::loadTexture("Resources/Textures/heightmap.png", TextureType::HeightMap, 0);
    // const unsigned rainDrop = ResourceLoader::loadTexture("Resources/Textures/raindrop.png", TextureType::AlbedoMap, 0);
    // const unsigned dirt     = ResourceLoader::loadTexture("Resources/Textures/dirt.png", TextureType::AlbedoMap, 0);
    //const unsigned cityTex  = ResourceLoader::loadTexture("Resources/Textures/City Tex.png", TextureType::AlbedoMap, 0);

    // auto grassMaterialInfo = ResourceLoader::createPBRInfo("Resources/Textures/Grass", { TextureType::AlbedoMap, TextureType::AOMap, TextureType::MetalicMap, TextureType::NormalMap, TextureType::RoughnessMap }, { 0, 0, 0, 0, 0 });
    // auto& grassMaterial = ResourceLoader::createPBR(grassMaterialInfo);
    // grassMaterial.setRepeatValue(10);
    // 
    // auto waterMaterialInfo = ResourceLoader::createPBRInfo("Resources/Textures/Water", { TextureType::AlbedoMap, TextureType::RoughnessMap, TextureType::NormalMap, TextureType::AOMap, TextureType::MetalicMap }, { 0, 0, 0, 0, 0 });
    // auto& waterMaterial = ResourceLoader::createPBR(waterMaterialInfo);
    // Materials::MatItemSingle<glm::vec4> waterAlbedo({ 0, 0, 1, 1 });
    // Materials::MatItemSingle<glm::vec3> waterEmission({ 0, 0, 0 });
    // Materials::MatItemSingle<float> waterMetalic(0.5f);
    // Materials::MatItemSingle<float> waterRoughness(0.5f);
    // Materials::MatItemSingle<float> waterAO(0.5f);
    // 
    // waterMaterial.setAlbedo(&waterAlbedo);
    // waterMaterial.setEmission(&waterEmission);
    // waterMaterial.setMetalic(&waterMetalic);
    // waterMaterial.setRoughness(&waterRoughness);
    // waterMaterial.setAO(&waterAO);
  
    timer.log();

    timer.start("Objects");


    // GameObject orb(glm::vec3(0, 1.5, 0));
    // Component::RenderMesh orbMesh = Component::RenderMesh();
    // orbMesh.setModel(orbBuffer);
    // orbMesh.setMaterial(&waterMaterial);
    // orb.addComponet(&orbMesh);

    /*Particles::DomeDistribution distribution = Particles::DomeDistribution(1.0f);
    Component::ParticleEmmiter emmiter = Component::ParticleEmmiter(10, 10, true, 0.25);
    Materials::MatItemSingle<glm::vec4> particleAlbedo(rainDrop);
    emmiter.setAlbedo(&particleAlbedo); m
    emmiter.setShape(&distribution);
    orb.addComponet(&emmiter);*/

    timer.log();


    timer.start("Player");
    GameObject player = GameObject(glm::vec3(0, 0, 5));
    Component::Camera playerCamera = Component::Camera();
    player.addComponet(&playerCamera);
    Component::CharacterController cc;
    player.addComponet(&cc);
    PlayerControler playerScript;
    player.addComponet(&playerScript);
    DebugScreen debugScript;
    player.addComponet(&debugScript);
    Component::AudioReciever recieverComp;
    player.addComponet(&recieverComp);

    timer.log();

    timer.start("Scene");

    GameScene scene;
    scene.setContext(&main);
    scene.setMainCamera(&playerCamera);
    scene.setBG({ 1, 0.5, 0.25 });
    scene.initalize();

    scene.addObject(&player);

    //scene.addObject(&orb);

    timer.log();

    // SKYBOX //
    timer.start("Skybox");
    ResourceLoader::loadCubeMap("Resources/Textures/DistantMtSB", ".png", 0);
    SkyBox sb = SkyBox("DistantMtSB.cm");
    scene.setSkyBox(&sb);
    timer.log();
    // SKYBOX //
    
    Gizmos::Sphere gizmo1 = Gizmos::Sphere({ 0, 4, 0 }, { 1, 0, 0 });
    gizmo1.setRadius(0.25);
    gizmo1.setThickness(2);
    Gizmos::GizmoRenderer::addGizmo(&gizmo1);

    Utils::log("Started Loop");
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