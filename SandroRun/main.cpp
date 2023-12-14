
#include "Starter.hpp"

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

    // Vertex descriptors
    VertexDescriptor VOverlay;
    VertexDescriptor VVColor;
    VertexDescriptor VMesh;

    // Pipelines
    Pipeline POverlay;
    Pipeline PVColor;
    Pipeline PMesh;
    Pipeline PSkybox;

    // Models
    Model<VertexOverlay> MSplash;
    Model<VertexVColor> MMoto;
    Model<VertexMesh> MRoad;
    Model<VertexMesh> MTerrain;
    Model<VertexMesh> MSkybox;
    Model<VertexMesh> MRail;

    // Descriptor sets
    DescriptorSet DSSplash;
    DescriptorSet DSGubo;
    DescriptorSet DSMoto;
    DescriptorSet DSRoad;
    DescriptorSet DSTerrain;
    DescriptorSet DSRailLeft;
    DescriptorSet DSRailRight;
    DescriptorSet DSSkybox;

    // Textures
    Texture TSplash;
    Texture TRoad;
    Texture TTerrain;
    Texture TRail;
    Texture TSkybox;

    // Uniform blocks
    OverlayUniformBlock uboSplash;
    GlobalUniformBlock gubo;
    MeshUniformBlock uboMoto;
    MeshUniformBlock uboRoad;
    MeshUniformBlock uboTerrain;
    SkyboxUniformBlock uboSkybox;
    MeshUniformBlock uboRail;

    // Other stuff
    int gameState = 0;
    glm::vec3 pos;
    float yaw, pitch, roll;
    float yawNew, pitchNew, rollNew;
    glm::vec3 cameraPosition, newCameraPosition;
    float speed;

    void setWindowParameters() override {
        windowWidth = 1280;
        windowHeight = 720;
        windowTitle = "Sandro Run";
        windowResizable = GLFW_TRUE;
        initialBackgroundColor = {0.0f, 1.0f, 1.0f, 1.0f};

        uniformBlocksInPool = 20;
        texturesInPool = 15;
        setsInPool = 20;

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
        PMesh.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/MeshFrag.spv", {&DSLGubo, &DSLMesh});
        PSkybox.init(this, &VMesh, "shaders/SkyboxVert.spv", "shaders/SkyboxFrag.spv", {&DSLSkybox});
        PSkybox.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, false);

        // Init Models
        skyboxModel();
        MMoto.init(this, &VVColor, "models/moto.colored.obj", OBJ);
        MRail.init(this, &VMesh, "models/guardrail.obj", OBJ);

        splashModel();
        roadModel();
        terrainModel();

        // Init textures
        TRail.init(this, "textures/guardrail.jpg");

        // Init other stuff
        pos = glm::vec3(0.0f, 0.0f, 0.0f);
        yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
        yawNew = 0.0f, pitchNew = 0.0f, rollNew = 0.0f;
        cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        speed = 0;
    }

    void pipelinesAndDescriptorSetsInit() {
        // Init pipelines
        POverlay.create();
        PSkybox.create();
        PVColor.create();
        PMesh.create();

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
    }

    void pipelinesAndDescriptorSetsCleanup() {
        // Cleanup pipelines
        POverlay.cleanup();
        PVColor.cleanup();
        PMesh.cleanup();
        PSkybox.cleanup();

        // Cleanup Descriptor Sets
        DSMoto.cleanup();
        DSRoad.cleanup();
        DSTerrain.cleanup();
        DSRailLeft.cleanup();
        DSRailRight.cleanup();
        DSSkybox.cleanup();
        DSSplash.cleanup();
        DSGubo.cleanup();
    }

    void localCleanup() {
        // Cleanup textures
        TSplash.cleanup();
        TRoad.cleanup();
        TTerrain.cleanup();
        TRail.cleanup();
        TSkybox.cleanup();

        // Cleanup models
        MSplash.cleanup();
        MMoto.cleanup();
        MRoad.cleanup();
        MTerrain.cleanup();
        MRail.cleanup();
        MSkybox.cleanup();

        // Cleanup descriptor sets layouts
        DSLOverlay.cleanup();
        DSLVColor.cleanup();
        DSLMesh.cleanup();
        DSLSkybox.cleanup();
        DSLGubo.cleanup();

        // Destroy pipelines
        POverlay.destroy();
        PVColor.destroy();
        PMesh.destroy();
        PSkybox.destroy();
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
    }

    void updateUniformBuffer(uint32_t currentImage) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        glm::mat4 ViewProj;
        glm::mat4 World;

        // main state machine implementation
        switch (gameState) {
            case 0: // initial state - show splash screen
                if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                    gameState = 1; // jump to the wait key state
                }
                break;
            case 1: // run
                updateCameraPosition(ViewProj, World);
                break;
        }


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
        uboMoto.mMat = World;
        uboMoto.mvpMat = ViewProj * uboMoto.mMat;
        uboMoto.nMat = glm::inverse(glm::transpose(uboMoto.mMat));
        DSMoto.map(currentImage, &uboMoto, sizeof(uboMoto), 0);

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

        uboSplash.visible = (gameState == 0) ? 1.0f : 0.0f;
        DSSplash.map(currentImage, &uboSplash, sizeof(uboSplash), 0);
    }

    void splashModel();

    void roadModel();

    void terrainModel();

    void skyboxModel();

    void updateCameraPosition(glm::mat4 &ViewProj, glm::mat4 &World);
};

#include "BuildModels.hpp"
#include "CameraHandle.hpp"


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