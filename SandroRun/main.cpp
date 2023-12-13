
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
    DescriptorSetLayout DSLGubo;
    DescriptorSetLayout DSLVColor;
    DescriptorSetLayout DSLMesh;
    DescriptorSetLayout DSLSkybox;

    // Vertex descriptors
    VertexDescriptor VVColor;
    VertexDescriptor VMesh;

    // Pipelines
    Pipeline PVColor;
    Pipeline PMesh;
    Pipeline PSkybox;

    // Models
    Model<VertexVColor> MMoto;
    Model<VertexMesh> MRoad;
    Model<VertexMesh> MTerrain;
    Model<VertexMesh> MSkybox;

    // Descriptor sets
    DescriptorSet DSGubo;
    DescriptorSet DSMoto;
    DescriptorSet DSRoad;
    DescriptorSet DSTerrain;
    DescriptorSet DSSkybox;

    // Textures
    Texture TRoad;
    Texture TTerrain;
    Texture TSkybox;

    // Uniform blocks
    GlobalUniformBlock gubo;
    MeshUniformBlock uboMoto;
    MeshUniformBlock uboRoad;
    MeshUniformBlock uboTerrain;
    SkyboxUniformBlock uboSkybox;

    void setWindowParameters() override {
        windowWidth = 1280;
        windowHeight = 720;
        windowTitle = "Sandro Run";
        windowResizable = GLFW_TRUE;
        initialBackgroundColor = {0.0f, 1.0f, 1.0f, 1.0f};

        uniformBlocksInPool = 5;
        texturesInPool = 3;
        setsInPool = 5;

        Ar = (float) windowWidth / (float) windowHeight;
    }

    void onWindowResize(int w, int h) override {
        Ar = (float) w / (float) h;
    }

    void localInit() override {

        // Init Descriptor Sets Layouts
        DSLVColor.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});
        DSLMesh.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_ALL_GRAPHICS},
                            {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        DSLSkybox.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_VERTEX_BIT},
                              {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        DSLGubo.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});

        // Init Vertex Descriptors
        VVColor.init(this, {{0, sizeof(VertexVColor), VK_VERTEX_INPUT_RATE_VERTEX}},
                     {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexVColor, pos),   sizeof(glm::vec3), POSITION},
                      {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexVColor, norm),  sizeof(glm::vec3), NORMAL},
                      {0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexVColor, color), sizeof(glm::vec3), COLOR}});
        VMesh.init(this, {{0, sizeof(VertexMesh), VK_VERTEX_INPUT_RATE_VERTEX}},
                   {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexMesh, pos),  sizeof(glm::vec3), POSITION},
                    {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexMesh, norm), sizeof(glm::vec3), NORMAL},
                    {0, 2, VK_FORMAT_R32G32_SFLOAT,    offsetof(VertexMesh, UV),   sizeof(glm::vec2), UV}});

        // Init Pipelines
        PVColor.init(this, &VVColor, "shaders/VColorVert.spv", "shaders/VColorFrag.spv", {&DSLGubo, &DSLVColor});
        PMesh.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/MeshFrag.spv", {&DSLGubo, &DSLMesh});
        PSkybox.init(this, &VMesh, "shaders/SkyboxVert.spv", "shaders/SkyboxFrag.spv", {&DSLSkybox});
        PSkybox.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, false);

        // Init Models
        skyboxModel();
        MMoto.init(this, &VVColor, "models/moto.colored.obj", OBJ);
        roadModel();
        MRoad.initMesh(this, &VMesh);

        terrainModel();
        MTerrain.initMesh(this, &VMesh);

        // Init textures
    }

    void pipelinesAndDescriptorSetsInit() override {
        // Init pipelines
        PSkybox.create();
        PVColor.create();
        PMesh.create();

        // Init Descriptor Sets
        DSMoto.init(this, &DSLVColor, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}});
        DSRoad.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                     {1, TEXTURE, 0,                        &TRoad}});
        DSGubo.init(this, &DSLGubo, {{0, UNIFORM, sizeof(GlobalUniformBlock), nullptr}});
        DSTerrain.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                        {1, TEXTURE, 0,                        &TTerrain}});
        DSSkybox.init(this, &DSLSkybox, {{0, UNIFORM, sizeof(SkyboxUniformBlock), nullptr},
                                         {1, TEXTURE, 0,                          &TSkybox}});

    }

    void pipelinesAndDescriptorSetsCleanup() override {
        // Cleanup pipelines
        PVColor.cleanup();
        PMesh.cleanup();
        PSkybox.cleanup();

        // Cleanup Descriptor Sets
        DSMoto.cleanup();
        DSRoad.cleanup();
        DSTerrain.cleanup();
        DSSkybox.cleanup();
        DSGubo.cleanup();
    }

    void localCleanup() override {
        // Cleanup textures
        TRoad.cleanup();
        TTerrain.cleanup();
        TSkybox.cleanup();

        // Cleanup models
        MMoto.cleanup();
        MRoad.cleanup();
        MTerrain.cleanup();
        MSkybox.cleanup();

        // Cleanup descriptor sets layouts
        DSLVColor.cleanup();
        DSLMesh.cleanup();
        DSLSkybox.cleanup();
        DSLGubo.cleanup();

        // Destroy pipelines
        PVColor.destroy();
        PMesh.destroy();
        PSkybox.destroy();
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) override {
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

        DSGubo.bind(commandBuffer, PMesh, 0, currentImage);
        PMesh.bind(commandBuffer);
        MTerrain.bind(commandBuffer);
        DSTerrain.bind(commandBuffer, PMesh, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MTerrain.indices.size()), 1, 0, 0, 0);

        PSkybox.bind(commandBuffer);
        MSkybox.bind(commandBuffer);
        DSSkybox.bind(commandBuffer, PSkybox, 0, currentImage);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MSkybox.indices.size()), 1, 0, 0, 0);
    }

    void updateUniformBuffer(uint32_t currentImage) override {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        glm::mat4 ViewProj;
        glm::mat4 World;
        static glm::vec3 camPos;

        updateCameraPosition(ViewProj, World, camPos);

        gubo.DlightDir = glm::normalize(glm::vec3(1.0f, 2.0f, 3.0f));
        gubo.DlightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        gubo.AmbLightColor = glm::vec3(0.1f);
        gubo.eyePos = camPos;
        DSGubo.map(currentImage, &gubo, sizeof(gubo), 0);

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
        uboRoad.mvpMat = ViewProj * World;
        uboRoad.mMat = World;
        uboRoad.nMat = glm::inverse(glm::transpose(World));
        DSRoad.map(currentImage, &uboRoad, sizeof(uboRoad), 0);

        World = glm::mat4(1.0f);
        uboTerrain.amb = 1.0f;
        uboTerrain.gamma = 180.0f;
        uboTerrain.sColor = glm::vec3(1.0f);
        uboTerrain.mvpMat = ViewProj * World;
        uboTerrain.mMat = World;
        uboTerrain.nMat = glm::inverse(glm::transpose(World));
        DSTerrain.map(currentImage, &uboTerrain, sizeof(uboTerrain), 0);

        World = glm::mat4(1.0f);
        uboSkybox.mMat = World;
        uboSkybox.nMat = glm::inverse(glm::transpose(World));
        uboSkybox.mvpMat = glm::mat3(ViewProj);
        DSSkybox.map(currentImage, &uboSkybox, sizeof(uboSkybox), 0);
    }

    void roadModel();

    void terrainModel();

    void skyboxModel();

    void updateCameraPosition(glm::mat4 &ViewProj, glm::mat4 &World, glm::vec3 &cameraPosition);
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