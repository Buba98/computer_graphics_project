#include "Parameters.hpp"

void SandroRun::controller() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Variables
    float deltaT, time;
    bool fire = false;
    glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
    glm::vec3 ux = glm::vec3(1, 0, 0);
    glm::vec3 uy = glm::vec3(0, 1, 0);
    glm::vec3 uz = glm::vec3(0, 0, -1);

    // Input acquisition
    getSixAxis(deltaT, m, r, fire, time);

    // Fire handling
    bool handleFire = (!(wasFire) && fire);
    if (handleFire)
        holdFire = !holdFire && scene.gameState;
    wasFire = fire;

    // Game beginning
    if (scene.gameState == 0) {
        if (handleFire) {
            std::cout << "Game started" << std::endl;
            scene.gameState = 1;
            scene.currText = 1;
            scene.startTime = time;
            createCommandBuffers();
        }
        return;
    } else if (scene.gameState == 2) {
        if (handleFire) {
            resetGame();
            std::cout << "Game restarted" << std::endl;
            createCommandBuffers();
        }
        return;
    }

    // Collisions detection
    if (!scene.gameOver) {
        checkCollisionsWithCars();
        checkCollisionsWithGuardRails();
    }

    // Game over
    if (scene.gameOver) {
        std::cout << "Game over" << std::endl;
        scene.gameState = 2;
        scene.splashVisibility = 1.0f;
        return;
    }

    // Daytime update
    bool n = false;
    if (glfwGetKey(window, GLFW_KEY_N)) {
        n = true;
    }
    if (n && !wasN) {
        scene.dayTime = (scene.dayTime + 1) % 3;
    }
    wasN = n;

    // Camera reset
    if (glfwGetKey(window, GLFW_KEY_R)) {
        camera.yaw = 0.0f;
        camera.yawNew = camera.yaw;
        camera.pitch = M_PI / 2.5f;
        camera.pitchNew = camera.pitch;
    }

    // Splash screen fading
    if (scene.splashVisibility != 0.0f) {
        scene.splashVisibility = glm::clamp(scene.splashVisibility - deltaT, 0.0f, 1.0f);
    }

    // Camera rotation
    camera.pitchNew += ROT_SPEED * r.x * deltaT;
    camera.pitchNew = glm::clamp(camera.pitchNew, MIN_PITCH, MAX_PITCH);
    camera.pitch = camera.pitch * glm::exp(-LAMBDA * deltaT) + camera.pitchNew * (1 - glm::exp(-LAMBDA * deltaT)); // Pitch damping
    camera.pitch = glm::clamp(camera.pitch, MIN_PITCH, MAX_PITCH);

    camera.yawNew += ROT_SPEED * r.y * deltaT;
    camera.yawNew = glm::clamp(camera.yawNew, MIN_YAW, MAX_YAW);
    camera.yaw = camera.yaw * glm::exp(-LAMBDA * deltaT) + camera.yawNew * (1 - glm::exp(-LAMBDA * deltaT)); // Yaw damping
    camera.yaw = glm::clamp(camera.yaw, MIN_YAW, MAX_YAW);

    // Score update
    int curr = (int) (abs(moto.pos.z) / ONE_POINT_SCORE_DISTANCE) + 1;
    if (scene.currText != curr) {
        scene.currText = curr;
        createCommandBuffers();
    }

    // Vehicles movement
    updateMoto(deltaT, time, m, ux, uz);
    updateCars(deltaT);
}

void SandroRun::viewHandler(glm::mat4 &ViewProj, glm::mat4 &World) {
    World = glm::translate(glm::mat4(1), moto.pos);
    ViewProj = glm::perspective(FOV_Y, Ar, NEAR_PLANE, FAR_PLANE);
    ViewProj[1][1] *= -1;

    if (holdFire) {
        // First person view
        ViewProj *= glm::rotate(glm::mat4(1.0), (float) (camera.pitch - M_PI / 2.5f), glm::vec3(1, 0, 0)) *
                    glm::rotate(glm::mat4(1.0), camera.yaw, glm::vec3(0, 1, 0)) *
                    glm::rotate(glm::mat4(1.0), -moto.roll / 2.0f, glm::vec3(0, 0, 1)) *
                    glm::translate(glm::mat4(1.0), -moto.pos + glm::vec3((-CAM_HEIGHT - .25f) * sin(-moto.roll),
                                                                    (-CAM_HEIGHT - .25f) * cos(moto.roll) -
                                                                    (CAM_HEIGHT / 2) * sin(moto.pitch),
                                                                    -sin(moto.pitch)));

        camera.pos = World * glm::vec4(0, CAM_HEIGHT, 0, 1);
    } else {
        // Third person view
        glm::mat4 rot =
                glm::rotate(glm::mat4(1), -camera.yaw, glm::vec3(0, 1, 0)) *
                glm::rotate(glm::mat4(1), camera.pitch, glm::vec3(1, 0, 0));
        camera.pos = World * rot * glm::vec4(0, CAM_DIST, 0, 1);
        glm::vec3 a = World * glm::vec4(0, 0, 0, 1) + glm::vec4(0, CAM_HEIGHT, 0, 0);

        ViewProj *= glm::lookAt(camera.pos, a, glm::vec3(0, 1, 0));
    }
}

void SandroRun::resetGame() {
    // Camera position and orientation
    camera.yaw = 0.0f, camera.pitch = M_PI / 2.5f;
    camera.yawNew = 0.0f, camera.pitchNew = M_PI / 2.5f;
    camera.pos = glm::vec3(0.0f, 0.0f, 0.0f);

    // Moto position and orientation
    moto.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    moto.speed = 0;
    moto.roll = 0;
    moto.pitch = 0;
    moto.wheelPitch = 0;

    // Game state
    scene.currText = 0;
    scene.gameState = 0;
    scene.splashVisibility = 1.0f;
    scene.frontWorldLimit = -WORLD_LENGTH;
    scene.backWorldLimit = 0;
    scene.dayTime = 0;
    scene.gameOver = false;
    scene.startTime = 0;
    wasN = false;

    // Cars positions
    initCars();
}
