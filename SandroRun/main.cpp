
#include "Starter.hpp"
#include "TextMaker.hpp"
#include "Parameters.hpp"

struct MeshUniformBlock {
    alignas(4) float amb;
    alignas(4) float gamma;
    alignas(16) glm::vec3 sColor;
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 mMat;
    alignas(16) glm::mat4 nMat;
};

struct CarMeshUniformBlock {
    alignas(4) int palette;
    alignas(4) float amb;
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
};

struct GlobalUniformBlock {
    alignas(16) glm::vec3 DlightDir;
    alignas(16) glm::vec3 DlightColor;
    alignas(16) glm::vec3 AmbLightColor;
    alignas(16) glm::vec3 eyePos;
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
    bool isGoingForward;
    float velocity;
};

class SandroRun : public BaseProject {
protected:
    float Ar;

    // Descriptor sets layouts
    DescriptorSetLayout DSLOverlay;
    DescriptorSetLayout DSLGubo;
    DescriptorSetLayout DSLVColor;
    DescriptorSetLayout DSLMesh;
    DescriptorSetLayout DSLSkybox;
    DescriptorSetLayout DSLCars;

    // Vertex descriptors
    VertexDescriptor VOverlay;
    VertexDescriptor VVColor;
    VertexDescriptor VMesh;

    // Pipelines
    Pipeline POverlay;
    Pipeline PVColor;
    Pipeline PMesh;
    Pipeline PSkybox;
    Pipeline PCars;
    Pipeline PTree;

    // Models
    Model<VertexOverlay> MSplash;
    Model<VertexVColor> MMoto;
    Model<VertexVColor> MFrontWheel;
    Model<VertexVColor> MRearWheel;
    Model<VertexMesh> MRoad;
    Model<VertexMesh> MTerrain;
    Model<VertexMesh> MSkybox;
    Model<VertexMesh> MRail;
    Model<VertexMesh> MCars[NUM_CAR_MODELS];
    Model<VertexVColor> MTrees[4];

    // Descriptor sets
    DescriptorSet DSSplash;
    DescriptorSet DSGubo;
    DescriptorSet DSMoto;
    DescriptorSet DSFrontWheel;
    DescriptorSet DSRearWheel;
    DescriptorSet DSRoad;
    DescriptorSet DSTerrain;
    DescriptorSet DSRails[2 * NUM_RAIL_PER_LINE];
    DescriptorSet DSSkybox;
    DescriptorSet DSCars[NUM_CAR_MODELS][NUM_CAR_MODEL_INSTANCES];
    DescriptorSet DSTrees[2 * 2 * NUM_TREE_PER_LINE];

    // Textures
    Texture TSplash;
    Texture TRoad;
    Texture TTerrain;
    Texture TRail;
    Texture TSkybox[3];
    Texture TCar[5];

    // Text
    TextMaker score;

    // Uniform blocks
    OverlayUniformBlock uboSplash;
    GlobalUniformBlock gubo;
    SkyboxUniformBlock uboSkybox;
    MeshUniformBlock ubo;
    CarMeshUniformBlock uboCar;

    // Other stuff
    std::vector<SingleText> texts;
    int gameState;
    int currText;
    glm::vec3 pos;
    float yaw, pitch, roll;
    float yawNew, pitchNew, rollNew;
    glm::vec3 cameraPosition;
    float speed;
    float motoRoll;
    float motoPitch;
    float wheelPitch;
    bool wasFire;
    bool holdFire;
    float splashVisibility;
    Car cars[NUM_CAR_MODELS][NUM_CAR_MODEL_INSTANCES];
    float frontWorldLimit, backWorldLimit;
    int dayTime;

    void setWindowParameters() {
        windowWidth = 1280;
        windowHeight = 720;
        windowTitle = "Sandro Run";
        windowResizable = GLFW_FALSE;
        initialBackgroundColor = {0.0f, 1.0f, 1.0f, 1.0f};

        uniformBlocksInPool = 100;
        texturesInPool = 100;
        setsInPool = 50 + NUM_CAR_MODELS * NUM_CAR_MODEL_INSTANCES;

        Ar = (float) windowWidth / (float) windowHeight;
    }

    void onWindowResize(int w, int h) {
        Ar = (float) w / (float) h;
    }

    void localInit() {
        // Init Descriptor Sets Layouts
        DSLOverlay.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_ALL_GRAPHICS},
                               {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        DSLVColor.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});
        DSLMesh.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_ALL_GRAPHICS},
                            {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        DSLSkybox.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_ALL_GRAPHICS},
                              {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                              {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                              {3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        DSLGubo.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});
        DSLCars.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_ALL_GRAPHICS},
                            {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                            {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                            {3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                            {4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                            {5, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});

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
        PVColor.init(this, &VVColor, "shaders/VColorVert.spv", "shaders/VColorFrag.spv", {&DSLGubo, &DSLVColor});
        PVColor.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, false);
        PMesh.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/MeshFrag.spv", {&DSLGubo, &DSLMesh});
        PSkybox.init(this, &VMesh, "shaders/SkyboxVert.spv", "shaders/SkyboxFrag.spv", {&DSLSkybox});
        PSkybox.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, false);
        PCars.init(this, &VMesh, "shaders/CarVert.spv", "shaders/CarFrag.spv", {&DSLGubo, &DSLCars});
        PTree.init(this, &VVColor, "shaders/VColorVert.spv", "shaders/VColorFrag.spv", {&DSLGubo, &DSLVColor});

        // Init Models
        MMoto.init(this, &VVColor, "models/moto.obj", OBJ);
        MFrontWheel.init(this, &VVColor, "models/frontWheel.obj", OBJ);
        MRearWheel.init(this, &VVColor, "models/rearWheel.obj", OBJ);
        MRail.init(this, &VMesh, "models/guardrail.obj", OBJ);
        for (int model = 0; model < NUM_CAR_MODELS; model++) {
            std::string modelFile = "models/cars/car_" + std::to_string(model + 1) + ".obj";
            MCars[model].init(this, &VMesh, modelFile, OBJ);
        }

        for (int i = 0; i < 4; ++i) {
            std::string objFile = "models/trees/tree" + std::to_string(i + 1) + ".obj";
            MTrees[i].init(this, &VVColor, objFile.c_str(), OBJ);
        }

        // Init textures
        TRail.init(this, "textures/guardrail.jpg");
        for (int i = 0; i < NUM_CAR_PALETTES; i++) {
            std::string paletteFile = "textures/car_palettes/" + std::to_string(i) + ".png";
            TCar[i].init(this, paletteFile.c_str());
        }

        // Custom inits

        splashModel();
        roadModel();
        terrainModel();
        skyboxModel();

        // Text
        texts.push_back({1, {"Sandro Run", "", "", ""}, 0, 0});
        for (int i = 0; i < 1000; ++i) {
            texts.push_back({2, {"Score: ", std::to_string(i), "", ""}, 0, 0});
        }

        score.init(this, &texts);

        // Init other stuff
        pos = glm::vec3(0.0f, 0.0f, 0.0f);
        yaw = 0.0f, pitch = M_PI / 2.5f, roll = 0.0f;
        yawNew = 0.0f, pitchNew = M_PI / 2.5f, rollNew = 0.0f;
        cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        speed = 0;
        motoRoll = 0;
        motoPitch = 0;
        wheelPitch = 0;
        wasFire = false;
        holdFire = false;
        currText = 0;
        gameState = 0;
        splashVisibility = 1.0f;
        frontWorldLimit = -WORLD_LENGTH;
        backWorldLimit = 0;
        dayTime = 0;

        // Cars initialization
        for (int m = 0; m < NUM_CAR_MODELS; m++) {
            for (int i = 0; i < NUM_CAR_MODEL_INSTANCES; i++) {
                cars[m][i].pos = glm::vec3(0.0f);
            }
        }
        for (int model = 0; model < NUM_CAR_MODELS; model++) {
            for (int i = 0; i < NUM_CAR_MODEL_INSTANCES; i++) {
                regenerateCar(model, i);
            }
        }
        for (int model = 0; model < NUM_CAR_MODELS; model++) {
            for (int i = 0; i < NUM_CAR_MODEL_INSTANCES; i++) {
                if (cars[model][i].isGoingForward)
                    cars[model][i].pos.z += (float) WORLD_LENGTH * 0.75f;
            }
        }
    }

    void pipelinesAndDescriptorSetsInit() {
        // Init pipelines
        PSkybox.create();
        POverlay.create();
        PVColor.create();
        PMesh.create();
        PCars.create();
        PTree.create();

        // Init Descriptor Sets
        DSSplash.init(this, &DSLOverlay, {{0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
                                          {1, TEXTURE, 0,                           &TSplash}});
        DSMoto.init(this, &DSLVColor, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}});
        DSFrontWheel.init(this, &DSLVColor, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}});
        DSRearWheel.init(this, &DSLVColor, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}});
        DSRoad.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                     {1, TEXTURE, 0,                        &TRoad}});
        DSTerrain.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                        {1, TEXTURE, 0,                        &TTerrain}});
        for (DescriptorSet &DSRail: DSRails) {
            DSRail.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                         {1, TEXTURE, 0,                        &TRail}});
        }
        DSSkybox.init(this, &DSLSkybox, {{0, UNIFORM, sizeof(SkyboxUniformBlock), nullptr},
                                         {1, TEXTURE, 0,                          &TSkybox[0]},
                                         {2, TEXTURE, 0,                          &TSkybox[1]},
                                         {3, TEXTURE, 0,                          &TSkybox[2]}});
        DSGubo.init(this, &DSLGubo, {{0, UNIFORM, sizeof(GlobalUniformBlock), nullptr}});
        for (int model = 0; model < NUM_CAR_MODELS; model++) {
            for (DescriptorSet &DSCar: DSCars[model])
                DSCar.init(this, &DSLCars, {{0, UNIFORM, sizeof(CarMeshUniformBlock), nullptr},
                                            {1, TEXTURE, 0,                           &TCar[0]},
                                            {2, TEXTURE, 0,                           &TCar[1]},
                                            {3, TEXTURE, 0,                           &TCar[2]},
                                            {4, TEXTURE, 0,                           &TCar[3]},
                                            {5, TEXTURE, 0,                           &TCar[4]}});;
        }
        for (DescriptorSet &DSTree: DSTrees) {
            DSTree.init(this, &DSLVColor, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}});
        }

        score.pipelinesAndDescriptorSetsInit();
    }

    void pipelinesAndDescriptorSetsCleanup() {
        // Cleanup pipelines
        POverlay.cleanup();
        PVColor.cleanup();
        PMesh.cleanup();
        PSkybox.cleanup();
        PCars.cleanup();
        PTree.cleanup();

        // Cleanup Descriptor Sets
        DSMoto.cleanup();
        DSFrontWheel.cleanup();
        DSRearWheel.cleanup();
        DSRoad.cleanup();
        DSTerrain.cleanup();
        for (DescriptorSet &DSRail: DSRails) {
            DSRail.cleanup();
        }
        DSSkybox.cleanup();
        DSSplash.cleanup();
        DSGubo.cleanup();
        for (int model = 0; model < NUM_CAR_MODELS; model++) {
            for (DescriptorSet &DSCar: DSCars[model])
                DSCar.cleanup();
        }
        for (DescriptorSet &DSTree: DSTrees) {
            DSTree.cleanup();
        }

        score.pipelinesAndDescriptorSetsCleanup();
    }

    void localCleanup() {
        // Cleanup textures
        TSplash.cleanup();
        TRoad.cleanup();
        TRail.cleanup();
        for (Texture &T: TSkybox) {
            T.cleanup();
        }
        TTerrain.cleanup();
        for (Texture &T: TCar) {
            T.cleanup();
        }

        // Cleanup models
        MSplash.cleanup();
        MMoto.cleanup();
        MFrontWheel.cleanup();
        MRearWheel.cleanup();
        MRoad.cleanup();
        MTerrain.cleanup();
        MRail.cleanup();
        MSkybox.cleanup();
        for (Model<VertexMesh> &MCar: MCars) {
            MCar.cleanup();
        }
        for (Model<VertexVColor> &MTree: MTrees) {
            MTree.cleanup();
        }

        // Cleanup descriptor sets layouts
        DSLOverlay.cleanup();
        DSLVColor.cleanup();
        DSLMesh.cleanup();
        DSLSkybox.cleanup();
        DSLGubo.cleanup();
        DSLCars.cleanup();

        // Destroy pipelines
        POverlay.destroy();
        PVColor.destroy();
        PMesh.destroy();
        PSkybox.destroy();
        PCars.destroy();
        PTree.destroy();

        score.localCleanup();
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
        POverlay.bind(commandBuffer);
        MSplash.bind(commandBuffer);
        DSSplash.bind(commandBuffer, POverlay, 0, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MSplash.indices.size()), 1, 0, 0, 0);

        DSGubo.bind(commandBuffer, PVColor, 0, currentImage);
        PVColor.bind(commandBuffer);
        MMoto.bind(commandBuffer);
        DSMoto.bind(commandBuffer, PVColor, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MMoto.indices.size()), 1, 0, 0, 0);

        MFrontWheel.bind(commandBuffer);
        DSFrontWheel.bind(commandBuffer, PVColor, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MFrontWheel.indices.size()), 1, 0, 0, 0);

        MRearWheel.bind(commandBuffer);
        DSRearWheel.bind(commandBuffer, PVColor, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MRearWheel.indices.size()), 1, 0, 0, 0);

        DSGubo.bind(commandBuffer, PMesh, 0, currentImage);
        PMesh.bind(commandBuffer);
        MRoad.bind(commandBuffer);
        DSRoad.bind(commandBuffer, PMesh, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MRoad.indices.size()), 1, 0, 0, 0);

        MTerrain.bind(commandBuffer);
        DSTerrain.bind(commandBuffer, PMesh, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTerrain.indices.size()), 1, 0, 0, 0);

        MRail.bind(commandBuffer);
        for (DescriptorSet DSRail: DSRails) {
            DSRail.bind(commandBuffer, PMesh, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MRail.indices.size()), 1, 0, 0, 0);
        }

        DSGubo.bind(commandBuffer, PCars, 0, currentImage);
        PCars.bind(commandBuffer);
        for (int model = 0; model < NUM_CAR_MODELS; model++) {
            for (DescriptorSet &DSCar: DSCars[model]) {
                MCars[model].bind(commandBuffer);
                DSCar.bind(commandBuffer, PCars, 1, currentImage);
                vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MCars[model].indices.size()), 1, 0, 0, 0);
            }
        }

        PSkybox.bind(commandBuffer);
        MSkybox.bind(commandBuffer);
        DSSkybox.bind(commandBuffer, PSkybox, 0, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MSkybox.indices.size()), 1, 0, 0, 0);

        DSGubo.bind(commandBuffer, PTree, 0, currentImage);
        PTree.bind(commandBuffer);
        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            MTrees[i % 2].bind(commandBuffer);
            DSTrees[i].bind(commandBuffer, PTree, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTrees[i % 2].indices.size()), 1, 0, 0, 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            MTrees[i % 2].bind(commandBuffer);
            DSTrees[i + NUM_TREE_PER_LINE].bind(commandBuffer, PTree, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTrees[i % 2].indices.size()), 1, 0, 0, 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            MTrees[(i % 2) + 2].bind(commandBuffer);
            DSTrees[i + 2 * NUM_TREE_PER_LINE].bind(commandBuffer, PTree, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTrees[(i % 2) + 2].indices.size()), 1, 0, 0, 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            MTrees[(i % 2) + 2].bind(commandBuffer);
            DSTrees[i + 3 * NUM_TREE_PER_LINE].bind(commandBuffer, PTree, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTrees[(i % 2) + 2].indices.size()), 1, 0, 0, 0);
        }

        score.populateCommandBuffer(commandBuffer, currentImage, currText);
    }

    void updateUniformBuffer(uint32_t currentImage) {
        controller();

        glm::mat4 ViewProj;
        glm::mat4 World;

        viewHandler(ViewProj, World);

        const int shift = pos.z / TERRAIN_LENGTH;

        backWorldLimit = (float) shift * TERRAIN_LENGTH;
        frontWorldLimit = backWorldLimit - WORLD_LENGTH;

        gubo.DlightDir = glm::normalize(glm::vec3(1.0f, 2.0f, 3.0f));
        gubo.DlightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        gubo.AmbLightColor = glm::vec3(0.1f);
        gubo.eyePos = cameraPosition;
        DSGubo.map(currentImage, &gubo, sizeof(gubo), 0);

        dayTime = -shift % 3;
        uboSkybox.time_of_day = dayTime;
        uboSkybox.mMat = glm::mat4(1.0f) * glm::translate(glm::mat4(1), cameraPosition);
        uboSkybox.nMat = glm::inverse(glm::transpose(uboSkybox.mMat));
        uboSkybox.mvpMat = ViewProj * uboSkybox.mMat;
        DSSkybox.map(currentImage, &uboSkybox, sizeof(uboSkybox), 0);

        const float OFFSET = .5f;

        ubo.amb = 1.0f;
        ubo.gamma = 180.0f;
        ubo.sColor = glm::vec3(1.0f);
        ubo.mMat = World * glm::translate(glm::mat4(1), glm::vec3(0, .315f - (.015f * sin(motoRoll)), OFFSET)) *
                   glm::rotate(glm::mat4(1), motoRoll, glm::vec3(0, 0, 1)) *
                   glm::rotate(glm::mat4(1), motoPitch, glm::vec3(1, 0, 0));
        ubo.mvpMat = ViewProj * ubo.mMat;
        ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
        DSMoto.map(currentImage, &ubo, sizeof(ubo), 0);

        const float DIST_WHEELS = 1.62f;

        ubo.mMat = World * glm::translate(glm::mat4(1), glm::vec3(DIST_WHEELS * sin(motoPitch) * sin(-motoRoll),
                                                                  .315f - (.015f * sin(motoRoll)) +
                                                                  DIST_WHEELS * sin(motoPitch) * cos(motoRoll),
                                                                  -DIST_WHEELS * cos(motoPitch) + OFFSET)) *
                   glm::rotate(glm::mat4(1), motoRoll, glm::vec3(0, 0, 1)) *
                   glm::rotate(glm::mat4(1), wheelPitch, glm::vec3(1, 0, 0)) *
                   glm::scale(glm::mat4(1), glm::vec3(0.78f));
        ubo.mvpMat = ViewProj * ubo.mMat;
        ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
        DSFrontWheel.map(currentImage, &ubo, sizeof(ubo), 0);

        ubo.mMat = World * glm::translate(glm::mat4(1), glm::vec3(0, .315f - (.015f * sin(motoRoll)), OFFSET)) *
                   glm::rotate(glm::mat4(1), motoRoll, glm::vec3(0, 0, 1)) *
                   glm::rotate(glm::mat4(1), wheelPitch, glm::vec3(1, 0, 0)) *
                   glm::rotate(glm::mat4(1), motoPitch, glm::vec3(1, 0, 0));
        ubo.mvpMat = ViewProj * ubo.mMat;
        ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
        DSRearWheel.map(currentImage, &ubo, sizeof(ubo), 0);

        uboCar.amb = 1.0f;
        uboCar.gamma = 180.0f;
        uboCar.sColor = glm::vec3(1.0f);
        for (int model = 0; model < NUM_CAR_MODELS; model++) {
            for (int index = 0; index < NUM_CAR_MODEL_INSTANCES; index++) {
                uboCar.palette = (model + index) % NUM_CAR_PALETTES;
                uboCar.mMat = glm::translate(glm::mat4(1), cars[model][index].pos) *
                              glm::rotate(glm::mat4(1), glm::radians(
                                                  cars[model][index].isGoingForward ? 0.0f : 180.0f),
                                          glm::vec3(0, 1, 0)) *
                              glm::scale(glm::mat4(1), glm::vec3(1.0f));
                uboCar.mvpMat = ViewProj * uboCar.mMat;
                uboCar.nMat = glm::inverse(glm::transpose(uboCar.mMat));
                DSCars[model][index].map(currentImage, &uboCar, sizeof(uboCar), 0);
            }
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(ROAD_WIDTH / 2 + TERRAIN_WIDTH / 2, 0, shift * 120.0f - 20 - i * 60));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSTrees[i].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(-ROAD_WIDTH / 2 - 3 * (TERRAIN_WIDTH / 2), 0,
                                                shift * 120.0f - 20 - i * 60));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSTrees[i + NUM_TREE_PER_LINE].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(-ROAD_WIDTH / 2 - TERRAIN_WIDTH / 2, 0, shift * 120.0f + 10 - i * 60));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSTrees[i + 2 * NUM_TREE_PER_LINE].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        for (int i = 0; i < NUM_TREE_PER_LINE; i++) {
            ubo.mMat = glm::translate(glm::mat4(1),
                                      glm::vec3(ROAD_WIDTH / 2 + 3 * (TERRAIN_WIDTH / 2), 0,
                                                shift * 120.0f + 10 - i * 60));
            ubo.mvpMat = ViewProj * ubo.mMat;
            ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
            DSTrees[i + 3 * NUM_TREE_PER_LINE].map(currentImage, &ubo, sizeof(ubo), 0);
        }

        ubo.mMat = glm::translate(glm::mat4(1), glm::vec3(0, 0, shift * TERRAIN_LENGTH));
        ubo.mvpMat = ViewProj * ubo.mMat;
        ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
        DSRoad.map(currentImage, &ubo, sizeof(ubo), 0);

        ubo.mMat = glm::translate(glm::mat4(1), glm::vec3(0, 0, shift * TERRAIN_LENGTH));
        ubo.mvpMat = ViewProj * ubo.mMat;
        ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
        DSTerrain.map(currentImage, &ubo, sizeof(ubo), 0);

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

        uboSplash.visible = splashVisibility;
        DSSplash.map(currentImage, &uboSplash, sizeof(uboSplash), 0);
    }

    void splashModel();

    void roadModel();

    void terrainModel();

    void skyboxModel();

    void controller();

    void viewHandler(glm::mat4 &ViewProj, glm::mat4 &World);

    void regenerateCar(int model, int index);
};

#include "BuildModels.hpp"
#include "HandleCommands.hpp"


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