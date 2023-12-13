
#include "Starter.hpp"

struct MeshUniformBlock {
    alignas(4) float amb;
    alignas(4) float gamma;
    alignas(16) glm::vec3 sColor;
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

    // Vertex descriptors
    VertexDescriptor VVColor;
    VertexDescriptor VMesh;

    // Pipelines
    Pipeline PVColor;
    Pipeline PMesh;

    // Models
    Model<VertexVColor> MMoto;
    Model<VertexMesh> MRoad;

    // Descriptor sets
    DescriptorSet DSGubo;
    DescriptorSet DSMoto;
    DescriptorSet DSRoad;

    // Textures
    Texture TRoad;

    // Uniform blocks
    GlobalUniformBlock gubo;
    MeshUniformBlock uboMoto;
    MeshUniformBlock uboRoad;

    void setWindowParameters() {
        windowWidth = 1280;
        windowHeight = 720;
        windowTitle = "Sandro Run";
        windowResizable = GLFW_TRUE;
        initialBackgroundColor = {0.0f, 1.0f, 1.0f, 1.0f};

        uniformBlocksInPool = 3;
        texturesInPool = 1;
        setsInPool = 3;


        Ar = (float) windowWidth / (float) windowHeight;
    }

    void onWindowResize(int w, int h) {
        Ar = (float) w / (float) h;
    }

    void localInit() {

        // Init Descriptor Sets Layouts
        DSLVColor.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});
        DSLMesh.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         VK_SHADER_STAGE_ALL_GRAPHICS},
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


        // Init Models
        MMoto.init(this, &VVColor, "models/moto.colored.obj", OBJ);
        MRoad.vertices = {{{0.0f,  0.0f, 0.0f},
                                  {0.0f, 1.0f, 0.0f},
                                  {0.0f, 0.0f}},
                          {{20.0f, 0.0f, 0.0f},
                                  {0.0f, 1.0f, 0.0f},
                                  {1.0f, 0.0f}},
                          {{0.0f,  0.0f, 10.0f},
                                  {0.0f, 1.0f, 0.0f},
                                  {0.0f, 1.0f}},
                          {{20.0f, 0.0f, 10.0f},
                                  {0.0f, 1.0f, 0.0f},
                                  {1.0f, 1.0f}}};
        MRoad.indices = {2, 1, 0, 1, 2, 3};
        MRoad.initMesh(this, &VMesh);

        // Init textures
        TRoad.init(this, "textures/road.png");
    }

    void pipelinesAndDescriptorSetsInit() {
        // Init pipelines
        PVColor.create();
        PMesh.create();

        // Init Descriptor Sets
        DSMoto.init(this, &DSLVColor, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}});
        DSRoad.init(this, &DSLMesh, {{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                                     {1, TEXTURE, 0,                        &TRoad}});
        DSGubo.init(this, &DSLGubo, {{0, UNIFORM, sizeof(GlobalUniformBlock), nullptr}});
    }

    void pipelinesAndDescriptorSetsCleanup() {
        // Cleanup pipelines
        PVColor.cleanup();
        PMesh.cleanup();

        // Cleanup Descriptor Sets
        DSMoto.cleanup();
        DSRoad.cleanup();
        DSGubo.cleanup();
    }

    void localCleanup() {
        // Cleanup textures
        TRoad.cleanup();

        // Cleanup models
        MMoto.cleanup();
        MRoad.cleanup();

        // Cleanup descriptor sets layouts
        DSLVColor.cleanup();
        DSLMesh.cleanup();
        DSLGubo.cleanup();

        // Destroy pipelines
        PVColor.destroy();
        PMesh.destroy();
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
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
    }

    void updateUniformBuffer(uint32_t currentImage) {
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
        uboMoto.mMat = World * glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
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
    }

    void updateCameraPosition(glm::mat4 &ViewProj, glm::mat4 &World, glm::vec3 &cameraPosition) {
        const float FOV_Y = glm::radians(45.0f);
        const float NEAR_PLANE = 0.1f;
        const float FAR_PLANE = 100.0f;

        const glm::vec3 STARTING_POS = glm::vec3(0.0f, 0.0f, 0.0f);

        const float CAM_HEIGHT = 0.75f;
        const float CAM_DIST = 3.5f;

        const float MIN_PITCH = glm::radians(-30.0f);
        const float MAX_PITCH = glm::radians(80.0f);

        const float ROT_SPEED = glm::radians(120.0f);
        const float MOV_SPEED = 3.0f;

        float deltaT;
        glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);

        bool fire = false;
        getSixAxis(deltaT, m, r, fire);

        // static bool wasFire = false;
        // bool handleFire = (wasFire && (!fire));
        // wasFire = fire;

        static glm::vec3 pos = STARTING_POS;
        static float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
        static float yawNew = 0.0f, pitchNew = 0.0f, rollNew = 0.0f;

        const float LAMBDA = 10.0f;

        static glm::vec3 newCameraPosition;
        glm::vec3 a;

        glm::vec3 ux = glm::vec3(glm::rotate(glm::mat4(1), -yaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1));
        glm::vec3 uy = glm::vec3(0, 1, 0);
        glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1), -yaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1));

        pitchNew += ROT_SPEED * r.x * deltaT;
        pitchNew = glm::clamp(pitchNew, MIN_PITCH, MAX_PITCH);
        pitch = pitch * glm::exp(-LAMBDA * deltaT) + pitchNew * (1 - glm::exp(-LAMBDA * deltaT)); // Pitch damping
        pitch = glm::clamp(pitch, MIN_PITCH, MAX_PITCH);

        yawNew += ROT_SPEED * r.y * deltaT;
        yaw = yaw * glm::exp(-LAMBDA * deltaT) + yawNew * (1 - glm::exp(-LAMBDA * deltaT)); // Yaw damping

        rollNew += ROT_SPEED * r.z * deltaT;
        roll = roll * glm::exp(-LAMBDA * deltaT) + rollNew * (1 - glm::exp(-LAMBDA * deltaT)); // Roll damping

        pos += ux * MOV_SPEED * m.x * deltaT;
        pos += uz * MOV_SPEED * m.z * deltaT;
        pos += uy * MOV_SPEED * m.y * deltaT;

        World = glm::translate(glm::mat4(1), pos) *
                glm::rotate(glm::mat4(1), -yaw, glm::vec3(0, 1, 0));

        newCameraPosition = World * glm::vec4(0, CAM_HEIGHT + CAM_DIST * glm::sin(pitch), CAM_DIST * glm::cos(pitch), 1);

        a = World * glm::vec4(0, 0, 0, 1) + glm::vec4(0, CAM_HEIGHT, 0, 0);

        cameraPosition = cameraPosition * glm::exp(-LAMBDA * deltaT) +
                         newCameraPosition * (1 - glm::exp(-LAMBDA * deltaT)); // Camera damping

        glm::mat4 View = glm::rotate(glm::mat4(1), -roll, glm::vec3(0, 0, 1)) * glm::lookAt(cameraPosition, a, uy);
        glm::mat4 Proj = glm::perspective(FOV_Y, Ar, NEAR_PLANE, FAR_PLANE);
        Proj[1][1] *= -1;
        ViewProj = Proj * View;
    }
};

// This is the main: probably you do not need to touch this!
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