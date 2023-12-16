
#include "Starter.hpp"
#include "TextMaker.hpp"

#define NUM_CAR1_INSTANCES 2

struct MeshUniformBlock {
    alignas(4) float amb;
    alignas(4) float gamma;
    alignas(16) glm::vec3 sColor;
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 mMat;
    alignas(16) glm::mat4 nMat;
};

struct SkyboxUniformBlock {
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

    // Models
    Model<VertexOverlay> MSplash;
    Model<VertexVColor> MMoto;
    Model<VertexMesh> MRoad;
    Model<VertexMesh> MTerrain;
    Model<VertexMesh> MSkybox;
    Model<VertexMesh> MRail;
    Model<VertexVColor> MCar1;

    // Descriptor sets
    DescriptorSet DSSplash;
    DescriptorSet DSGubo;
    DescriptorSet DSMoto;
    DescriptorSet DSRoad;
    DescriptorSet DSTerrain;
    DescriptorSet DSRailLeft;
    DescriptorSet DSRailRight;
    DescriptorSet DSSkybox;
    DescriptorSet DSCar1[NUM_CAR1_INSTANCES];

    // Textures
    Texture TSplash;
    Texture TRoad;
    Texture TTerrain;
    Texture TRail;
    Texture TSkybox;

    // Text
    TextMaker score;

    // Uniform blocks
    OverlayUniformBlock uboSplash;
    GlobalUniformBlock gubo;
    MeshUniformBlock uboMoto;
    MeshUniformBlock uboRoad;
    MeshUniformBlock uboTerrain;
    SkyboxUniformBlock uboSkybox;
    MeshUniformBlock uboRail;
    MeshUniformBlock uboCar1[NUM_CAR1_INSTANCES];

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
    bool wasFire;
    bool holdFire;
    float splashVisibility;

    void setWindowParameters() override {
        windowWidth = 1280;
        windowHeight = 720;
        windowTitle = "Sandro Run";
        windowResizable = GLFW_FALSE;
        initialBackgroundColor = {0.0f, 1.0f, 1.0f, 1.0f};

        uniformBlocksInPool = 50;
        texturesInPool = 50;
        setsInPool = 50;

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
        DSLSkybox.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_VERTEX_BIT},
                              {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        DSLGubo.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});
        DSLCars.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});

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
        PSkybox.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, false);
        PCars.init(this, &VVColor, "shaders/VColorVert.spv", "shaders/VColorFrag.spv", {&DSLGubo, &DSLVColor});

        // Init Models
        skyboxModel();
        MMoto.init(this, &VVColor, "models/moto.obj", OBJ);
        MRail.init(this, &VMesh, "models/guardrail.obj", OBJ);
        MCar1.init(this, &VVColor, "models/cars/car1.obj", OBJ);

        splashModel();
        roadModel();
        terrainModel();

        // Init textures
        TRail.init(this, "textures/guardrail.jpg");

        // Text
        texts.push_back(
                {1, {"Sandro Run", "", "", ""}, 0, 0}
        );
        for (int i = 0; i < 1000; ++i) {
            std::string str = std::to_string(i);
            char *c_str = new char[str.length() + 1];
            strcpy(c_str, str.c_str());
            texts.push_back(
                    {2, {"Score: ", c_str, "", ""}, 0, 0}
            );
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
        wasFire = false;
        holdFire = false;
        currText = 0;
        gameState = 0;
        splashVisibility = 1.0f;
    }

    void pipelinesAndDescriptorSetsInit() {
        // Init pipelines
        POverlay.create();
        PSkybox.create();
        PVColor.create();
        PMesh.create();
        PCars.create();

        // Init Descriptor Sets
        DSSplash.init(this, &DSLOverlay, {{0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
                                          {1, TEXTURE, 0,                           &TSplash}});
        DSMoto.init(this, &DSLVColor, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}});
        DSRoad.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                     {1, TEXTURE, 0,                        &TRoad}});
        DSTerrain.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                        {1, TEXTURE, 0,                        &TTerrain}});
        DSRailLeft.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                         {1, TEXTURE, 0,                        &TRail}});
        DSRailRight.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                          {1, TEXTURE, 0,                        &TRail}});
        DSSkybox.init(this, &DSLSkybox, {{0, UNIFORM, sizeof(SkyboxUniformBlock), nullptr},
                                         {1, TEXTURE, 0,                          &TSkybox}});
        DSGubo.init(this, &DSLGubo, {{0, UNIFORM, sizeof(GlobalUniformBlock), nullptr}});
        for(auto & item : DSCar1)
            item.init(this, &DSLVColor, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}});;

        score.pipelinesAndDescriptorSetsInit();
    }

    void pipelinesAndDescriptorSetsCleanup() {
        // Cleanup pipelines
        POverlay.cleanup();
        PVColor.cleanup();
        PMesh.cleanup();
        PSkybox.cleanup();
        PCars.cleanup();

        // Cleanup Descriptor Sets
        DSMoto.cleanup();
        DSRoad.cleanup();
        DSTerrain.cleanup();
        DSRailLeft.cleanup();
        DSRailRight.cleanup();
        DSSkybox.cleanup();
        DSSplash.cleanup();
        DSGubo.cleanup();
        for(auto & item : DSCar1)
            item.cleanup();

        score.pipelinesAndDescriptorSetsCleanup();
    }

    void localCleanup() {
        // Cleanup textures
        TSplash.cleanup();
        TRoad.cleanup();
        TRail.cleanup();
        TSkybox.cleanup();
        TTerrain.cleanup();

        // Cleanup models
        MSplash.cleanup();
        MMoto.cleanup();
        MRoad.cleanup();
        MTerrain.cleanup();
        MRail.cleanup();
        MSkybox.cleanup();
        MCar1.cleanup();

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

        DSGubo.bind(commandBuffer, PMesh, 0, currentImage);
        PMesh.bind(commandBuffer);
        MRoad.bind(commandBuffer);
        DSRoad.bind(commandBuffer, PMesh, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MRoad.indices.size()), 1, 0, 0, 0);

        MTerrain.bind(commandBuffer);
        DSTerrain.bind(commandBuffer, PMesh, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTerrain.indices.size()), 1, 0, 0, 0);

        MRail.bind(commandBuffer);
        DSRailLeft.bind(commandBuffer, PMesh, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MRail.indices.size()), 1, 0, 0, 0);

        DSRailRight.bind(commandBuffer, PMesh, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MRail.indices.size()), 1, 0, 0, 0);

        PSkybox.bind(commandBuffer);
        MSkybox.bind(commandBuffer);
        DSSkybox.bind(commandBuffer, PSkybox, 0, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MSkybox.indices.size()), 1, 0, 0, 0);

        DSGubo.bind(commandBuffer, PCars, 0, currentImage);
        PCars.bind(commandBuffer);
        for(auto & item : DSCar1){
            MCar1.bind(commandBuffer);
            item.bind(commandBuffer, PCars, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MCar1.indices.size()), 1, 0, 0, 0);
        }

        score.populateCommandBuffer(commandBuffer, currentImage, currText);
    }

    void updateUniformBuffer(uint32_t currentImage) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        glm::mat4 ViewProj;
        glm::mat4 World;

        handleCommands(ViewProj, World);

        int shift = pos.z / 120;

        gubo.DlightDir = glm::normalize(glm::vec3(1.0f, 2.0f, 3.0f));
        gubo.DlightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        gubo.AmbLightColor = glm::vec3(0.1f);
        gubo.eyePos = cameraPosition;
        DSGubo.map(currentImage, &gubo, sizeof(gubo), 0);

        uboSkybox.mMat = glm::mat4(1.0f) * glm::translate(glm::mat4(1), cameraPosition);
        uboSkybox.nMat = glm::inverse(glm::transpose(uboSkybox.mMat));
        uboSkybox.mvpMat = ViewProj * uboSkybox.mMat;
        DSSkybox.map(currentImage, &uboSkybox, sizeof(uboSkybox), 0);

        uboMoto.amb = 1.0f;
        uboMoto.gamma = 180.0f;
        uboMoto.sColor = glm::vec3(1.0f);
        uboMoto.mMat = World * glm::translate(glm::mat4(1), glm::vec3(0, .315f - (.015f * sin(motoRoll)), .5f)) *
                       glm::rotate(glm::mat4(1), motoRoll, glm::vec3(0, 0, 1)) *
                       glm::rotate(glm::mat4(1), motoPitch, glm::vec3(1, 0, 0));
        uboMoto.mvpMat = ViewProj * uboMoto.mMat;
        uboMoto.nMat = glm::inverse(glm::transpose(uboMoto.mMat));
        DSMoto.map(currentImage, &uboMoto, sizeof(uboMoto), 0);

        World = glm::mat4(1.0f);
        const float RIGHT_LANE = 6.5f, CENTER_RIGHT_LANE = 2.2f, CENTER_LEFT_LANE = -CENTER_RIGHT_LANE, LEFT_LANE = -RIGHT_LANE;
        glm::vec3 car1Positions[NUM_CAR1_INSTANCES] = {glm::vec3(CENTER_LEFT_LANE, 0, pos.z - 15), glm::vec3(RIGHT_LANE, 0, pos.z - 30)};
        bool car1GoingForward[NUM_CAR1_INSTANCES] = {false, true};
        for (int index = 0; index < NUM_CAR1_INSTANCES; index++){
            uboCar1[index].amb = 1.0f;
            uboCar1[index].gamma = 180.0f;
            uboCar1[index].sColor = glm::vec3(1.0f);
            uboCar1[index].mMat = World * glm::translate(glm::mat4(1), car1Positions[index]) *
                                  glm::rotate(glm::mat4(1), glm::radians(car1GoingForward[index] ? 0.0f : 180.0f), glm::vec3(0,1,0)) *
                                  glm::scale(glm::mat4(1), glm::vec3(0.375f));
            uboCar1[index].mvpMat = ViewProj * uboCar1[index].mMat;
            uboCar1[index].nMat = glm::inverse(glm::transpose(uboCar1[index].mMat));
            DSCar1[index].map(currentImage, &uboCar1[index], sizeof(uboCar1[index]), 0);
        }

        World = glm::mat4(1.0f);
        uboRoad.amb = 1.0f;
        uboRoad.gamma = 180.0f;
        uboRoad.sColor = glm::vec3(1.0f);
        uboRoad.mMat = World * glm::translate(glm::mat4(1), glm::vec3(0, 0, shift * 120));
        uboRoad.mvpMat = ViewProj * uboRoad.mMat;
        uboRoad.nMat = glm::inverse(glm::transpose(uboRoad.mMat));
        DSRoad.map(currentImage, &uboRoad, sizeof(uboRoad), 0);

        World = glm::mat4(1.0f);
        uboTerrain.amb = 1.0f;
        uboTerrain.gamma = 180.0f;
        uboTerrain.sColor = glm::vec3(1.0f);
        uboTerrain.mMat = World * glm::translate(glm::mat4(1), glm::vec3(0, 0, shift * 120));
        uboTerrain.mvpMat = ViewProj * uboTerrain.mMat;
        uboTerrain.nMat = glm::inverse(glm::transpose(uboTerrain.mMat));
        DSTerrain.map(currentImage, &uboTerrain, sizeof(uboTerrain), 0);

        World = glm::mat4(1.0f);
        uboRail.amb = 1.0f;
        uboRail.gamma = 180.0f;
        uboRail.sColor = glm::vec3(1.0f);
        uboRail.mMat = World * glm::translate(glm::mat4(1), glm::vec3(-10, 0, shift * 120)) *
                       glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0)) *
                       glm::scale(glm::mat4(1), glm::vec3(0.21555f));
        uboRail.mvpMat = ViewProj * uboRail.mMat;
        uboRail.nMat = glm::inverse(glm::transpose(uboRail.mMat));
        DSRailLeft.map(currentImage, &uboRail, sizeof(uboRail), 0);

        uboRail.mMat = World * glm::translate(glm::mat4(1), glm::vec3(10, 0, -230 + shift * 120)) *
                       glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(0, 1, 0)) *
                       glm::scale(glm::mat4(1), glm::vec3(0.21555f));
        uboRail.mvpMat = ViewProj * uboRail.mMat;
        uboRail.nMat = glm::inverse(glm::transpose(uboRail.mMat));
        DSRailRight.map(currentImage, &uboRail, sizeof(uboRail), 0);

        uboSplash.visible = splashVisibility;
        DSSplash.map(currentImage, &uboSplash, sizeof(uboSplash), 0);
    }

    void splashModel();

    void roadModel();

    void terrainModel();

    void skyboxModel();

    void handleCommands(glm::mat4 &ViewProj, glm::mat4 &World);
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