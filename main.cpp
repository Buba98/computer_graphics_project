
#include "Starter.hpp"
#include "TextMaker.hpp"
#include "Parameters.hpp"
#include "Audio.hpp"
#include <thread>

struct MeshUniformBlock {
    alignas(4) float gamma;
    alignas(16) glm::vec3 sColor;
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 mMat;
    alignas(16) glm::mat4 nMat;
};

struct CarMeshUniformBlock {
    alignas(4) int palette;
    alignas(4) float gamma;
    alignas(16) glm::vec3 sColor;
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 mMat;
    alignas(16) glm::mat4 nMat;
};

struct SkyboxUniformBlock {
    alignas(4) int time_of_day;
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 mMat;
    alignas(16) glm::mat4 nMat;
};

struct OverlayUniformBlock {
    alignas(4) float visible;
    alignas(4) int splashSelector;
};

struct HUDUniformBlock {
    alignas(4) float visible;
    alignas(4) float speed;
    alignas(4) float ar;
};

struct GlobalUniformBlock {
    alignas(4) int dayTime;
    alignas(4) int shift;
    alignas(16) glm::vec3 DlightDir;
    alignas(16) glm::vec3 DlightColor;
    alignas(16) glm::vec3 AmbLightColor;
    alignas(16) glm::vec3 eyePos;
    alignas(16) glm::vec3 motoPos;
    alignas(16) glm::vec3 motoDir;
};

struct VertexMesh {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 UV;
};

struct VertexOverlay {
    glm::vec2 pos;
    glm::vec2 UV;
};

struct VertexVColor {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec3 color;
};

struct Car {
    glm::vec3 pos;
    int palette;
    bool isGoingForward;
    float speed;
    float length, width;
};

struct Moto {
    glm::vec3 pos;
    float roll;
    float pitch;
    float wheelPitch;
    float speed;
};

struct Camera {
    glm::vec3 pos;
    float yaw, yawNew;
    float pitch, pitchNew;
    float roll, rollNew;
};

struct Scene {
    int gameState;
    float frontWorldLimit, backWorldLimit;
    int currText;
    float splashVisibility;
    int dayTime;
    float startTime;
};

class SandroRun : public BaseProject {
public:
    ~SandroRun() {
        sound_state.mutex.lock();
        sound_state.stop = true;
        sound_state.mutex.unlock();

        audioThread.join();
    }

protected:
    float Ar;

    // Descriptor sets layouts
    DescriptorSetLayout DSLOverlay;
    DescriptorSetLayout DSLHUD;
    DescriptorSetLayout DSLGubo;
    DescriptorSetLayout DSLMoto;
    DescriptorSetLayout DSLMesh;
    DescriptorSetLayout DSLSkybox;

    // Vertex descriptors
    VertexDescriptor VOverlay;
    VertexDescriptor VVColor;
    VertexDescriptor VMesh;

    // Pipelines
    Pipeline POverlay;
    Pipeline PHUD;
    Pipeline PMoto;
    Pipeline PMesh;
    Pipeline PSkybox;
    Pipeline PCar;
    Pipeline PLandscape;
    Pipeline PStreet;

    // Models
    Model<VertexOverlay> MSplash;
    Model<VertexOverlay> MSpeedometer;
    Model<VertexVColor> MMotos[2];
    Model<VertexVColor> MFrontWheel;
    Model<VertexVColor> MRearWheel;
    Model<VertexMesh> MMotoLight;
    Model<VertexMesh> MRoad;
    Model<VertexMesh> MTerrain;
    Model<VertexMesh> MSkybox;
    Model<VertexMesh> MRail;
    Model<VertexMesh> MCars[NUM_CAR_MODELS];
    Model<VertexMesh> MTrees[4];
    Model<VertexMesh> MStreetlight;

    // Descriptor sets
    DescriptorSet DSOverlay;
    DescriptorSet DSSpeedometer;
    DescriptorSet DSGubo;
    DescriptorSet DSMoto;
    DescriptorSet DSFrontWheel;
    DescriptorSet DSRearWheel;
    DescriptorSet DSMotoLight;
    DescriptorSet DSRoad;
    DescriptorSet DSTerrain;
    DescriptorSet DSRails[NUM_TOT_RAILS];
    DescriptorSet DSSkybox;
    DescriptorSet DSCars[NUM_CAR_MODELS];
    DescriptorSet DSTrees[NUM_TOT_TREES];
    DescriptorSet DSStreetlights[NUM_TOT_STREETLIGHTS];

    // Textures
    Texture TSplashStart;
    Texture TSplashEnd;
    Texture TSpeedometer;
    Texture TSpeedometerHand;
    Texture TBrokenGlass;
    Texture TMotoLight[2];
    Texture TRoad;
    Texture TTerrain;
    Texture TRail;
    Texture TSkybox[3];
    Texture TCar[2];
    Texture TStreetlight[2];
    Texture TNoEmission;
    Texture TTree;

    // Text
    TextMaker score;

    // Uniform blocks
    OverlayUniformBlock uboOverlay;
    HUDUniformBlock uboHUD;
    GlobalUniformBlock gubo;
    SkyboxUniformBlock uboSkybox;
    MeshUniformBlock ubo;
    CarMeshUniformBlock uboCar;

    // Other stuff
    std::vector<SingleText> texts;
    bool wasFire, holdFire;
    bool wasN, wasR;
    bool wasC, holdC;
    bool wasP, holdP;
    Scene scene;
    Camera camera;
    Moto moto;
    Car cars[NUM_CAR_MODELS];
    std::thread audioThread;
    Sound_State sound_state;
    Audio audio = Audio(&sound_state);

    void setWindowParameters() {
        windowWidth = 1280;
        windowHeight = 720;
        windowTitle = "Sandro Run";
        windowResizable = GLFW_FALSE;
        initialBackgroundColor = {0.0f, 1.0f, 1.0f, 1.0f};

        uniformBlocksInPool = 76;
        texturesInPool = 147;
        setsInPool = 77;

        Ar = (float) windowWidth / (float) windowHeight;
    }

    void onWindowResize(int w, int h) {
        Ar = (float) w / (float) h;
    }

    void localInit() {
        // Init Descriptor Sets Layouts
        DSLOverlay.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_ALL_GRAPHICS},
                               {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                               {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                               {3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        DSLHUD.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_ALL_GRAPHICS},
                           {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                           {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        DSLMoto.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});
        DSLMesh.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_ALL_GRAPHICS},
                            {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                            {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        DSLSkybox.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_ALL_GRAPHICS},
                              {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                              {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                              {3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        DSLGubo.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});

        // Init Vertex Descriptors
        VOverlay.init(this, {{0, sizeof(VertexOverlay), VK_VERTEX_INPUT_RATE_VERTEX}},
                      {{0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexOverlay, pos), sizeof(glm::vec2), OTHER},
                       {0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexOverlay, UV),  sizeof(glm::vec2), UV}});
        VVColor.init(this, {{0, sizeof(VertexVColor), VK_VERTEX_INPUT_RATE_VERTEX}},
                     {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexVColor, pos),   sizeof(glm::vec3), POSITION},
                      {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexVColor, norm),  sizeof(glm::vec3), NORMAL},
                      {0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexVColor, color), sizeof(glm::vec3), COLOR}});
        VMesh.init(this, {{0, sizeof(VertexMesh), VK_VERTEX_INPUT_RATE_VERTEX}},
                   {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexMesh, pos),  sizeof(glm::vec3), POSITION},
                    {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexMesh, norm), sizeof(glm::vec3), NORMAL},
                    {0, 2, VK_FORMAT_R32G32_SFLOAT,    offsetof(VertexMesh, UV),   sizeof(glm::vec2), UV}});

        // Init Pipelines
        POverlay.init(this, &VOverlay, "shaders/OverlayVert.spv", "shaders/OverlayFrag.spv", {&DSLOverlay});
        POverlay.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, false);
        PHUD.init(this, &VOverlay, "shaders/HUDVert.spv", "shaders/HUDFrag.spv", {&DSLHUD});
        PHUD.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, false);
        PMoto.init(this, &VVColor, "shaders/MotoVert.spv", "shaders/MotoFrag.spv", {&DSLGubo, &DSLMoto});
        PMesh.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/MeshFrag.spv", {&DSLGubo, &DSLMesh});
        PSkybox.init(this, &VMesh, "shaders/SkyboxVert.spv", "shaders/SkyboxFrag.spv", {&DSLSkybox});
        PSkybox.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, false);
        PCar.init(this, &VMesh, "shaders/CarVert.spv", "shaders/CarFrag.spv", {&DSLGubo, &DSLMesh});
        PLandscape.init(this, &VMesh, "shaders/LandscapeVert.spv", "shaders/LandscapeFrag.spv", {&DSLGubo, &DSLMesh});
        PStreet.init(this, &VMesh, "shaders/StreetVert.spv", "shaders/StreetFrag.spv", {&DSLGubo, &DSLMesh});

        // Init Models
        MMotos[0].init(this, &VVColor, "models/moto/moto_high.obj", OBJ);
        MMotos[1].init(this, &VVColor, "models/moto/moto_low.obj", OBJ);
        MFrontWheel.init(this, &VVColor, "models/moto/frontWheel.obj", OBJ);
        MRearWheel.init(this, &VVColor, "models/moto/rearWheel.obj", OBJ);
        MMotoLight.init(this, &VMesh, "models/moto/motoLight.obj", OBJ);
        MRail.init(this, &VMesh, "models/guardrail.obj", OBJ);
        for (int model = 0; model < NUM_CAR_MODELS; model++) {
            std::string modelFile = "models/cars/car_" + std::to_string(model + 1) + ".obj";
            MCars[model].init(this, &VMesh, modelFile, OBJ);
        }
        MStreetlight.init(this, &VMesh, "models/streetlight.obj", OBJ);

        for (int i = 0; i < 4; ++i) {
            std::string objFile = "models/trees/tree" + std::to_string(i + 1) + ".obj";
            MTrees[i].init(this, &VMesh, objFile, OBJ);
        }

        // Init textures
        TMotoLight[0].init(this, "textures/moto/moto_headlight.jpg");
        TMotoLight[1].init(this, "textures/moto/moto_headlightEmission.png");
        TRail.init(this, "textures/guardrail.jpg");
        TCar[0].init(this, "textures/car_palettes/palette.png");
        TCar[1].init(this, "textures/car_palettes/palette_emission.png");
        TStreetlight[0].init(this, "textures/streetlight/streetlight.png");
        TStreetlight[1].init(this, "textures/streetlight/streetlightEmission.png");
        TTree.init(this, "textures/tree.png");
        TNoEmission.init(this, "textures/no_emission.png");
        TBrokenGlass.init(this, "textures/broken_glass.png");

        // Custom inits
        initSplashModel();
        initHUD();
        initRoadModel();
        initTerrainModel();
        initSkyboxModel();

        // Text
        texts.push_back({1, {"Sandro Run", "", "", ""}, 0, 0});
        for (int i = 0; i < 100; ++i) {
            std::string text = "Score: " + std::to_string(i);
            texts.push_back({1, {text, "", "", ""}, 0, 0});
        }
        texts.push_back({1, {"You won! Endless run", "", "", ""}, 0, 0});

        score.init(this, &texts);

        // Init other stuff
        resetGame();
        wasFire = false, holdFire = false;
        wasN = false, wasR = false;
        wasP = false, holdP = false;
        wasC = false, holdC = false;
        audioThread = std::thread(&Audio::start, &audio);
    }

    void pipelinesAndDescriptorSetsInit() {
        // Init pipelines
        PSkybox.create();
        POverlay.create();
        PHUD.create();
        PMoto.create();
        PMesh.create();
        PCar.create();
        PLandscape.create();
        PStreet.create();

        // Init Descriptor Sets
        DSOverlay.init(this, &DSLOverlay, {{0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
                                           {1, TEXTURE, 0,                           &TSplashStart},
                                           {2, TEXTURE, 0,                           &TBrokenGlass},
                                           {3, TEXTURE, 0,                           &TSplashEnd}});
        DSSpeedometer.init(this, &DSLHUD, {{0, UNIFORM, sizeof(HUDUniformBlock), nullptr},
                                           {1, TEXTURE, 0,                       &TSpeedometer},
                                           {2, TEXTURE, 0,                       &TSpeedometerHand}});
        DSMoto.init(this, &DSLMoto, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}});
        DSFrontWheel.init(this, &DSLMoto, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}});
        DSRearWheel.init(this, &DSLMoto, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}});
        DSMotoLight.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                          {1, TEXTURE, 0,                        &TMotoLight[0]},
                                          {2, TEXTURE, 0,                        &TMotoLight[1]}});
        DSRoad.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                     {1, TEXTURE, 0,                        &TRoad},
                                     {2, TEXTURE, 0,                        &TNoEmission}});
        DSTerrain.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                        {1, TEXTURE, 0,                        &TTerrain},
                                        {2, TEXTURE, 0,                        &TNoEmission}});
        for (DescriptorSet &DSRail: DSRails) {
            DSRail.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                         {1, TEXTURE, 0,                        &TRail},
                                         {2, TEXTURE, 0,                        &TNoEmission}});
        }
        DSSkybox.init(this, &DSLSkybox, {{0, UNIFORM, sizeof(SkyboxUniformBlock), nullptr},
                                         {1, TEXTURE, 0,                          &TSkybox[0]},
                                         {2, TEXTURE, 0,                          &TSkybox[1]},
                                         {3, TEXTURE, 0,                          &TSkybox[2]}});
        DSGubo.init(this, &DSLGubo, {{0, UNIFORM, sizeof(GlobalUniformBlock), nullptr}});
        for (DescriptorSet &DSCar: DSCars) {
            DSCar.init(this, &DSLMesh, {{0, UNIFORM, sizeof(CarMeshUniformBlock), nullptr},
                                        {1, TEXTURE, 0,                           &TCar[0]},
                                        {2, TEXTURE, 0,                           &TCar[1]}});;
        }
        for (DescriptorSet &DSTree: DSTrees) {
            DSTree.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                         {1, TEXTURE, 0,                        &TTree},
                                         {2, TEXTURE, 0,                        &TNoEmission}});
        }
        for (DescriptorSet &DSStreetlight: DSStreetlights) {
            DSStreetlight.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                                {1, TEXTURE, 0,                        &TStreetlight[0]},
                                                {2, TEXTURE, 0,                        &TStreetlight[1]}});
        }

        score.pipelinesAndDescriptorSetsInit();
    }

    void pipelinesAndDescriptorSetsCleanup() {
        // Cleanup pipelines
        POverlay.cleanup();
        PHUD.cleanup();
        PMoto.cleanup();
        PMesh.cleanup();
        PSkybox.cleanup();
        PCar.cleanup();
        PLandscape.cleanup();
        PStreet.cleanup();

        // Cleanup Descriptor Sets
        DSMoto.cleanup();
        DSFrontWheel.cleanup();
        DSRearWheel.cleanup();
        DSMotoLight.cleanup();
        DSRoad.cleanup();
        DSTerrain.cleanup();
        for (DescriptorSet &DSRail: DSRails) {
            DSRail.cleanup();
        }
        DSSkybox.cleanup();
        DSOverlay.cleanup();
        DSSpeedometer.cleanup();
        DSGubo.cleanup();
        for (DescriptorSet &DSCar: DSCars) {
            DSCar.cleanup();
        }
        for (DescriptorSet &DSTree: DSTrees) {
            DSTree.cleanup();
        }
        for (DescriptorSet &DSStreetlight: DSStreetlights) {
            DSStreetlight.cleanup();
        }

        score.pipelinesAndDescriptorSetsCleanup();
    }

    void localCleanup() {
        // Cleanup textures
        TSplashStart.cleanup();
        TSplashEnd.cleanup();
        TSpeedometer.cleanup();
        TSpeedometerHand.cleanup();
        for (Texture &T: TMotoLight) {
            T.cleanup();
        }
        TRoad.cleanup();
        TRail.cleanup();
        for (Texture &T: TSkybox) {
            T.cleanup();
        }
        TTerrain.cleanup();
        for (Texture &T: TCar) {
            T.cleanup();
        }
        for (Texture &T: TStreetlight) {
            T.cleanup();
        }
        TTree.cleanup();
        TBrokenGlass.cleanup();
        TNoEmission.cleanup();

        // Cleanup models
        MSplash.cleanup();
        MSpeedometer.cleanup();
        for (Model<VertexVColor> &MMoto: MMotos) {
            MMoto.cleanup();
        }
        MFrontWheel.cleanup();
        MRearWheel.cleanup();
        MMotoLight.cleanup();
        MRoad.cleanup();
        MTerrain.cleanup();
        MRail.cleanup();
        MSkybox.cleanup();
        for (Model<VertexMesh> &MCar: MCars) {
            MCar.cleanup();
        }
        for (Model<VertexMesh> &MTree: MTrees) {
            MTree.cleanup();
        }
        MStreetlight.cleanup();

        // Cleanup descriptor sets layouts
        DSLOverlay.cleanup();
        DSLHUD.cleanup();
        DSLMoto.cleanup();
        DSLMesh.cleanup();
        DSLSkybox.cleanup();
        DSLGubo.cleanup();

        // Destroy pipelines
        POverlay.destroy();
        PHUD.destroy();
        PMoto.destroy();
        PMesh.destroy();
        PSkybox.destroy();
        PCar.destroy();
        PLandscape.destroy();
        PStreet.destroy();

        score.localCleanup();
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
        // Splash screen
        POverlay.bind(commandBuffer);
        MSplash.bind(commandBuffer);
        DSOverlay.bind(commandBuffer, POverlay, 0, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MSplash.indices.size()), 1, 0, 0, 0);

        // HUD
        PHUD.bind(commandBuffer);
        MSpeedometer.bind(commandBuffer);
        DSSpeedometer.bind(commandBuffer, PHUD, 0, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MSpeedometer.indices.size()), 1, 0, 0, 0);

        // Moto
        DSGubo.bind(commandBuffer, PMoto, 0, currentImage);
        PMoto.bind(commandBuffer);
        MMotos[holdC ? 0 : 1].bind(commandBuffer);
        DSMoto.bind(commandBuffer, PMoto, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MMotos[holdC ? 0 : 1].indices.size()), 1, 0, 0, 0);

        MFrontWheel.bind(commandBuffer);
        DSFrontWheel.bind(commandBuffer, PMoto, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MFrontWheel.indices.size()), 1, 0, 0, 0);

        if (!holdC) {
            MRearWheel.bind(commandBuffer);
            DSRearWheel.bind(commandBuffer, PMoto, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MRearWheel.indices.size()), 1, 0, 0, 0);
        }
        // Moto light
        DSGubo.bind(commandBuffer, PMesh, 0, currentImage);
        PMesh.bind(commandBuffer);
        if (!holdC) {
            MMotoLight.bind(commandBuffer);
            DSMotoLight.bind(commandBuffer, PMesh, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MMotoLight.indices.size()), 1, 0, 0, 0);
        }

        // Streetlights
        MStreetlight.bind(commandBuffer);
        for (DescriptorSet DSStreetlight: DSStreetlights) {
            DSStreetlight.bind(commandBuffer, PMesh, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MStreetlight.indices.size()), 1, 0, 0, 0);
        }

        // Rails
        MRail.bind(commandBuffer);
        for (DescriptorSet DSRail: DSRails) {
            DSRail.bind(commandBuffer, PMesh, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MRail.indices.size()), 1, 0, 0, 0);
        }

        // Road
        DSGubo.bind(commandBuffer, PStreet, 0, currentImage);
        PStreet.bind(commandBuffer);
        MRoad.bind(commandBuffer);
        DSRoad.bind(commandBuffer, PStreet, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MRoad.indices.size()), 1, 0, 0, 0);

        // Terrain
        DSGubo.bind(commandBuffer, PLandscape, 0, currentImage);
        PLandscape.bind(commandBuffer);
        MTerrain.bind(commandBuffer);
        DSTerrain.bind(commandBuffer, PLandscape, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTerrain.indices.size()), 1, 0, 0, 0);

        // Trees
        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            MTrees[i % 2].bind(commandBuffer);
            DSTrees[i].bind(commandBuffer, PLandscape, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTrees[i % 2].indices.size()), 1, 0, 0, 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            MTrees[i % 2].bind(commandBuffer);
            DSTrees[i + NUM_TREE_PER_LINE].bind(commandBuffer, PLandscape, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTrees[i % 2].indices.size()), 1, 0, 0, 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            MTrees[(i % 2) + 2].bind(commandBuffer);
            DSTrees[i + 2 * NUM_TREE_PER_LINE].bind(commandBuffer, PLandscape, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTrees[(i % 2) + 2].indices.size()), 1, 0, 0, 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            MTrees[(i % 2) + 2].bind(commandBuffer);
            DSTrees[i + 3 * NUM_TREE_PER_LINE].bind(commandBuffer, PLandscape, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTrees[(i % 2) + 2].indices.size()), 1, 0, 0, 0);
        }

        // Cars
        DSGubo.bind(commandBuffer, PCar, 0, currentImage);
        PCar.bind(commandBuffer);
        for (int model = 0; model < NUM_CAR_MODELS; model++) {
            MCars[model].bind(commandBuffer);
            DSCars[model].bind(commandBuffer, PCar, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MCars[model].indices.size()), 1, 0, 0, 0);
        }

        // Skybox
        PSkybox.bind(commandBuffer);
        MSkybox.bind(commandBuffer);
        DSSkybox.bind(commandBuffer, PSkybox, 0, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MSkybox.indices.size()), 1, 0, 0, 0);

        // Text
        score.populateCommandBuffer(commandBuffer, currentImage, scene.currText);
    }

    void updateUniformBuffer(uint32_t currentImage) {
        glm::mat4 ViewProj, World;

        // Game update
        controller();
        viewHandler(ViewProj, World);

        // World limits update
        const int shift = (int) moto.pos.z / TERRAIN_LENGTH;
        scene.backWorldLimit = (float) shift * TERRAIN_LENGTH;
        scene.frontWorldLimit = scene.backWorldLimit - WORLD_LENGTH;

        // Global uniform buffer
        switch (scene.dayTime) {
            case DAY:
                gubo.dayTime = DAY;
                gubo.DlightDir = glm::normalize(glm::vec3(2.0f, 3.0f, -3.0f));
                gubo.DlightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) * 0.8f;
                gubo.AmbLightColor = glm::vec3(0.1f);
                break;
            case SUNSET:
                gubo.dayTime = SUNSET;
                gubo.DlightDir = glm::normalize(glm::vec3(0.0f, 2.0f, -3.0f));
                gubo.DlightColor = glm::vec4(0.93f, 0.68f, 0.38f, 1.0f) * 0.3f;
                gubo.AmbLightColor = glm::vec3(0.93f, 0.68f, 0.38f) * 0.1f;
                break;
            case NIGHT:
                gubo.dayTime = NIGHT;
                gubo.DlightDir = glm::normalize(glm::vec3(1.0f, 2.0f, -3.0f));
                gubo.DlightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) * 0.005f;
                gubo.AmbLightColor = glm::vec3(0.0f);
                break;
        }

        gubo.motoDir = glm::normalize(glm::vec3({0, -sin(moto.pitch) + M_PI_(8), cos(moto.pitch)}));
        gubo.motoPos =
                moto.pos + glm::vec3(-MOTO_HEIGHT * sin(moto.roll) / 2, MOTO_HEIGHT * cos(moto.roll), -MOTO_LENGTH);
        gubo.shift = shift;
        gubo.eyePos = camera.pos;
        DSGubo.map(currentImage, &gubo, sizeof(gubo), 0);

        // Skybox
        uboSkybox.time_of_day = scene.dayTime;
        uboSkybox.mMat = glm::mat4(1.0f) * glm::translate(glm::mat4(1), camera.pos);
        uboSkybox.nMat = glm::inverse(glm::transpose(uboSkybox.mMat));
        uboSkybox.mvpMat = ViewProj * uboSkybox.mMat;
        DSSkybox.map(currentImage, &uboSkybox, sizeof(uboSkybox), 0);

        const float OFFSET = .5f;

        ubo.sColor = glm::vec3(1.0f);

        // Moto
        ubo.gamma = 180.0f;
        ubo.mMat = World * glm::translate(glm::mat4(1), glm::vec3(0, .315f - (.015f * sin(moto.roll)), OFFSET)) *
                   glm::rotate(glm::mat4(1), moto.roll, glm::vec3(0, 0, 1)) *
                   glm::rotate(glm::mat4(1), moto.pitch, glm::vec3(1, 0, 0));
        ubo.mvpMat = ViewProj * ubo.mMat;
        ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
        DSMoto.map(currentImage, &ubo, sizeof(ubo), 0);

        // Front wheel
        ubo.gamma = 180.0f;
        ubo.mMat = World * glm::translate(glm::mat4(1), glm::vec3(DIST_WHEELS * sin(moto.pitch) * sin(-moto.roll),
                                                                  .315f - (.015f * sin(moto.roll)) +
                                                                  DIST_WHEELS * sin(moto.pitch) * cos(moto.roll),
                                                                  -DIST_WHEELS * cos(moto.pitch) + OFFSET)) *
                   glm::rotate(glm::mat4(1), moto.roll, glm::vec3(0, 0, 1)) *
                   glm::rotate(glm::mat4(1), moto.wheelPitch, glm::vec3(1, 0, 0));
        ubo.mvpMat = ViewProj * ubo.mMat;
        ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
        DSFrontWheel.map(currentImage, &ubo, sizeof(ubo), 0);


        if (!holdC) {
            // Rear wheel
            ubo.gamma = 180.0f;
            ubo.mMat = World * glm::translate(glm::mat4(1), glm::vec3(0, .315f - (.015f * sin(moto.roll)), OFFSET)) *
                       glm::rotate(glm::mat4(1), moto.roll, glm::vec3(0, 0, 1)) *
                       glm::rotate(glm::mat4(1), moto.wheelPitch, glm::vec3(1, 0, 0)) *
                       glm::rotate(glm::mat4(1), moto.pitch, glm::vec3(1, 0, 0));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSRearWheel.map(currentImage, &ubo, sizeof(ubo), 0);

            // Moto light
            ubo.gamma = 180.0f;
            ubo.mMat = World * glm::translate(glm::mat4(1), glm::vec3(0, .315f - (.015f * sin(moto.roll)), OFFSET)) *
                       glm::rotate(glm::mat4(1), moto.roll, glm::vec3(0, 0, 1)) *
                       glm::rotate(glm::mat4(1), moto.pitch, glm::vec3(1, 0, 0));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSMotoLight.map(currentImage, &ubo, sizeof(ubo), 0);
        }

        // Cars
        uboCar.gamma = 60.0f;
        uboCar.sColor = glm::vec3(1.0f);
        for (int model = 0; model < NUM_CAR_MODELS; model++) {
            uboCar.palette = cars[model].palette;
            uboCar.mMat = glm::translate(glm::mat4(1), cars[model].pos) *
                          glm::rotate(glm::mat4(1), glm::radians(
                                              cars[model].isGoingForward ? 0.0f : 180.0f),
                                      glm::vec3(0, 1, 0));
            uboCar.mvpMat = ViewProj * uboCar.mMat;
            uboCar.nMat = glm::inverse(glm::transpose(uboCar.mMat));
            DSCars[model].map(currentImage, &uboCar, sizeof(uboCar), 0);
        }

        // Trees
        ubo.gamma = 180.0f;
        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(ROAD_WIDTH / 2 + TERRAIN_WIDTH / 2, 0,
                                                (float) shift * 120.0f - 20 - (float) i * 60));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSTrees[i].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(-ROAD_WIDTH / 2 - 3 * (TERRAIN_WIDTH / 2), 0,
                                                (float) shift * 120.0f - 20 - (float) i * 60));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSTrees[i + NUM_TREE_PER_LINE].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(-ROAD_WIDTH / 2 - TERRAIN_WIDTH / 2, 0,
                                                (float) shift * 120.0f + 10 - (float) i * 60));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSTrees[i + 2 * NUM_TREE_PER_LINE].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(ROAD_WIDTH / 2 + 3 * (TERRAIN_WIDTH / 2), 0,
                                                (float) shift * 120.0f + 10 - (float) i * 60));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSTrees[i + 3 * NUM_TREE_PER_LINE].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        // Road
        ubo.gamma = 180.0f;
        ubo.mMat = glm::translate(glm::mat4(1), glm::vec3(0, 0, shift * TERRAIN_LENGTH));
        ubo.mvpMat = ViewProj * ubo.mMat;
        ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
        DSRoad.map(currentImage, &ubo, sizeof(ubo), 0);

        // Terrain
        ubo.gamma = 180.0f;
        ubo.mMat = glm::translate(glm::mat4(1), glm::vec3(0, 0, shift * TERRAIN_LENGTH));
        ubo.mvpMat = ViewProj * ubo.mMat;
        ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
        DSTerrain.map(currentImage, &ubo, sizeof(ubo), 0);

        // Guardrails
        ubo.gamma = 180.0f;
        for (int i = 0; i < NUM_RAIL_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(-ROAD_WIDTH / 2, 0, -i * TERRAIN_LENGTH + shift * TERRAIN_LENGTH)) *
                       glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSRails[i].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        for (int i = 0; i < NUM_RAIL_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(ROAD_WIDTH / 2, 0, -TERRAIN_LENGTH + 10 - i * TERRAIN_LENGTH +
                                                                   shift * TERRAIN_LENGTH)) *
                       glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(0, 1, 0));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSRails[i + NUM_RAIL_PER_LINE].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        // Streetlights
        ubo.gamma = 180.0f;
        for (int i = 0; i < NUM_LIGHTS_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(-ROAD_WIDTH / 2 - 3.0, 0, 10 - i * TERRAIN_LENGTH / 2 +
                                                                          shift * TERRAIN_LENGTH));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSStreetlights[i].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        for (int i = 0; i < NUM_LIGHTS_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(ROAD_WIDTH / 2 + 3.0, 0,
                                                10 - TERRAIN_LENGTH / 4 - i * TERRAIN_LENGTH / 2 +
                                                shift * TERRAIN_LENGTH)) *
                       glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSStreetlights[i + NUM_LIGHTS_PER_LINE].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        // Splash screen
        uboOverlay.visible = scene.splashVisibility;
        uboOverlay.splashSelector = scene.gameState;
        DSOverlay.map(currentImage, &uboOverlay, sizeof(uboOverlay), 0);

        // HUD
        uboHUD.visible = scene.gameState == GAME_SCREEN;
        uboHUD.speed = moto.speed;
        uboHUD.ar = Ar;
        DSSpeedometer.map(currentImage, &uboHUD, sizeof(uboHUD), 0);
    }

    // Models
    void initSplashModel();

    void initRoadModel();

    void initTerrainModel();

    void initSkyboxModel();

    void initHUD();

    // Game
    void controller();

    void viewHandler(glm::mat4 &ViewProj, glm::mat4 &World);

    void gameOverAnimation(float deltaT);

    void mainGame(float deltaT, float time, glm::vec3 m, glm::vec3 r, glm::vec3 ux, glm::vec3 uz);

    void resetGame();

    // Vehicles
    void regenerateCar(Car &car);

    void initCars();

    void updateCars(float deltaT);

    void updateMoto(float deltaT, float time, glm::vec3 m, glm::vec3 ux, glm::vec3 uz);

    // Collisions
    bool checkCollisionsWithCars();

    bool checkCollisionsWithGuardRails();

    // Audio
    void refreshAudio();
};

#include "BuildModels.hpp"
#include "HandleCommands.hpp"
#include "Vehicles.hpp"

int main() {
    SandroRun app;

    try {
        app.run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}