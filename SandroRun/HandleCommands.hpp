#include "Parameters.hpp"

void SandroRun::controller() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Variables
    float deltaT, time;
    bool fire = false;
    glm::vec3 m = glm::vec3(0.0f);
    glm::vec3 r = glm::vec3(0.0f);
    glm::vec3 ux = glm::vec3(1, 0, 0);
    glm::vec3 uy = glm::vec3(0, 1, 0);
    glm::vec3 uz = glm::vec3(0, 0, -1);

    // Input acquisition
    getSixAxis(deltaT, m, r, fire, time);

    // Fire handling
    bool handleFire = (!(wasFire) && fire);
    if (handleFire) {
        holdFire = !holdFire && scene.gameState;
    }
    wasFire = fire;

    // Game beginning
    switch (scene.gameState) {
        case INITIAL_SCREEN:
            if (handleFire) {
                std::cout << "Game started" << std::endl;
                scene.gameState = GAME_SCREEN;
                scene.currText = 1;
                scene.startTime = time;
                createCommandBuffers();
            }
            break;
        case GAME_SCREEN:
            mainGame(deltaT, time, m, r, ux, uz);
            break;
        case GAME_OVER_ANIMATION:
            gameOverAnimation(deltaT);
            if (abs(moto.roll) >= (float) M_PI_2) {
                std::cout << "Game over" << std::endl;
                scene.gameState = GAME_OVER_SCREEN;
                scene.splashVisibility = 1.0f;
            }
            break;
        case GAME_OVER_SCREEN:
            if (handleFire) {
                resetGame();
                std::cout << "Game restarted" << std::endl;
                createCommandBuffers();
            }
            break;
    }
}

void SandroRun::mainGame(float deltaT, float time, glm::vec3 m, glm::vec3 r, glm::vec3 ux, glm::vec3 uz) {
    // Collisions detection
    if (!holdP) {
        if (checkCollisionsWithCars() || checkCollisionsWithGuardRails()) {
            scene.gameState = GAME_OVER_ANIMATION;
            return;
        }
    }

    // Daytime update
    bool n = glfwGetKey(window, GLFW_KEY_N);
    if (n && !wasN) {
        scene.dayTime = (scene.dayTime + 1) % 3;
    }
    wasN = n;

    // Presentation mode
    bool p = glfwGetKey(window, GLFW_KEY_P);
    bool handleP = (!(wasP) && p);
    if (handleP) {
        holdP = !holdP;
    }
    wasP = p;

    // Change camera mode
    bool c = glfwGetKey(window, GLFW_KEY_C);
    bool handleC = (!(wasC) && c);
    if (handleC) {
        holdC = !holdC;
        if (scene.gameState == GAME_SCREEN) {
            createCommandBuffers();
        }
    }
    wasC = c;

    // Camera reset
    if (glfwGetKey(window, GLFW_KEY_R)) {
        camera.yaw = STARTING_YAW;
        camera.yawNew = STARTING_YAW;
        camera.pitch = STARTING_PITCH;
        camera.pitchNew = STARTING_PITCH;
    }

    // Splash screen fading
    if (scene.splashVisibility != 0.0f) {
        scene.splashVisibility = glm::clamp(scene.splashVisibility - deltaT, 0.0f, 1.0f);
    }

    // Camera rotation
    camera.pitchNew += ROT_SPEED * r.x * deltaT;
    if (!holdP) {
        camera.pitchNew = glm::clamp(camera.pitchNew, MIN_PITCH, MAX_PITCH);
    }
    camera.pitch = camera.pitch * glm::exp(-LAMBDA * deltaT) +
                   camera.pitchNew * (1 - glm::exp(-LAMBDA * deltaT)); // Pitch damping
    if (!holdP) {
        camera.pitch = glm::clamp(camera.pitch, MIN_PITCH, MAX_PITCH);
    }

    camera.yawNew += ROT_SPEED * r.y * deltaT;
    if (!holdP) {
        camera.yawNew = glm::clamp(camera.yawNew, MIN_YAW, MAX_YAW);
    }
    camera.yaw =
            camera.yaw * glm::exp(-LAMBDA * deltaT) + camera.yawNew * (1 - glm::exp(-LAMBDA * deltaT)); // Yaw damping
    if (!holdP) {
        camera.yaw = glm::clamp(camera.yaw, MIN_YAW, MAX_YAW);
    }
    // Score update
    int curr = (int) (abs(moto.pos.z) / ONE_POINT_SCORE_DISTANCE) + 1;
    if (scene.currText != curr && scene.currText != 101) {
        scene.currText = curr > 101 ? 101 : curr;
        createCommandBuffers();
    }

    // Vehicles movement
    updateMoto(deltaT, time, m, ux, uz);
    updateCars(deltaT);

    camera.rollNew = moto.roll;
    camera.roll = camera.roll * glm::exp(-LAMBDA * deltaT) +
                  camera.rollNew * (1 - glm::exp(-LAMBDA * deltaT)); // Yaw damping
}

void SandroRun::viewHandler(glm::mat4 &ViewProj, glm::mat4 &World) {
    World = glm::translate(glm::mat4(1), moto.pos);
    ViewProj = glm::perspective(FOV_Y, Ar, NEAR_PLANE, FAR_PLANE);
    ViewProj[1][1] *= -1;

    if (holdC) {
        // First person view
        ViewProj *= glm::rotate(glm::mat4(1.0), (float) (camera.pitch - STARTING_PITCH), glm::vec3(1, 0, 0)) *
                    glm::rotate(glm::mat4(1.0), camera.yaw, glm::vec3(0, 1, 0)) *
                    glm::rotate(glm::mat4(1.0), -camera.roll / 2.0f, glm::vec3(0, 0, 1)) *
                    glm::translate(glm::mat4(1.0), -moto.pos + glm::vec3((-CAM_HEIGHT - .25f) * sin(-camera.roll),
                                                                         (-CAM_HEIGHT - .25f) * cos(camera.roll) -
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
    camera.yaw = STARTING_YAW;
    camera.yawNew = STARTING_YAW;
    camera.pitch = STARTING_PITCH;
    camera.pitchNew = STARTING_PITCH;
    camera.pos = glm::vec3(0.0f, 0.0f, 0.0f);

    // Moto position and orientation
    moto.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    moto.speed = 0;
    moto.roll = 0;
    moto.pitch = 0;
    moto.wheelPitch = 0;

    // Game state
    scene.currText = 0;
    scene.gameState = INITIAL_SCREEN;
    scene.splashVisibility = 1.0f;
    scene.frontWorldLimit = -WORLD_LENGTH;
    scene.backWorldLimit = 0;
    scene.dayTime = DAY;
    scene.startTime = 0;
    wasN = false;
    wasC = false;

    // Cars positions
    initCars();
}
