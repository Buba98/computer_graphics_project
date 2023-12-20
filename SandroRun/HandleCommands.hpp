#include "Parameters.hpp"

void SandroRun::controller() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Variables
    float deltaT;
    float time;
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
        holdFire = !holdFire && gameState;
    wasFire = fire;

    // Games states
    if (gameState == 0) {
        if (handleFire) {
            std::cout << "Game started" << std::endl;
            gameState = 1;
            currText = 1;
            createCommandBuffers();
        }
        return;
    } else if (gameState == 2) {
        if (handleFire) {
            resetGame();
            std::cout << "Game restarted" << std::endl;
            createCommandBuffers();
        }
        return;
    }

    // Game over
    if (glfwGetKey(window, GLFW_KEY_5))
        gameOver = true;

    if (gameOver) {
        std::cout << "Game over" << std::endl;
        gameState = 2;
        splashVisibility = 1.0f;
        return;
    }

    // Daytime update
    bool n = false;
    if (glfwGetKey(window, GLFW_KEY_N)) {
        n = true;
    }

    if (n && !wasN) {
        dayTime = (dayTime + 1) % 3;
    }
    wasN = n;

    // Splash screen fading
    if (splashVisibility != 0.0f) {
        splashVisibility = glm::clamp(splashVisibility - deltaT, 0.0f, 1.0f);
    }

    // Camera rotation
    pitchNew += ROT_SPEED * r.x * deltaT;
    pitchNew = glm::clamp(pitchNew, MIN_PITCH, MAX_PITCH);
    pitch = pitch * glm::exp(-LAMBDA * deltaT) + pitchNew * (1 - glm::exp(-LAMBDA * deltaT)); // Pitch damping
    pitch = glm::clamp(pitch, MIN_PITCH, MAX_PITCH);

    yawNew += ROT_SPEED * r.y * deltaT;
    yawNew = glm::clamp(yawNew, MIN_YAW, MAX_YAW);
    yaw = yaw * glm::exp(-LAMBDA * deltaT) + yawNew * (1 - glm::exp(-LAMBDA * deltaT)); // Yaw damping
    yaw = glm::clamp(yaw, MIN_YAW, MAX_YAW);

    // Moto rotation
    motoRoll = motoRoll - m.x * deltaT * 5.0f;
    if (m.x * deltaT == 0.0f) {
        motoRoll = motoRoll * (1 - glm::exp(-MOTO_ROLL_SPEED * deltaT)); // motoRoll damping
        if (motoRoll > 0.001 * MIN_MOTO_ROLL && motoRoll < 0.001 * MAX_MOTO_ROLL)
            motoRoll = 0.0f;
    }
    motoRoll = glm::clamp(motoRoll, MIN_MOTO_ROLL, MAX_MOTO_ROLL);

    motoPitch = motoPitch - m.z * deltaT * 2.0f;
    if (m.z * deltaT == 0.0f) {
        motoPitch = motoPitch * (1 - glm::exp(-MOTO_PITCH_SPEED * deltaT)); // motoPitch damping
        if (motoPitch < 0.001 * MAX_MOTO_PITCH)
            motoPitch = 0.0f;
    }
    motoPitch = glm::clamp(motoPitch, MIN_MOTO_PITCH, MAX_MOTO_PITCH);

    // Moto position
    speed = Z_SPEED * (log(time * .1f + 1) + 1) * .1f;
    pos += ux * X_SPEED * deltaT * sin(-motoRoll);
    pos += uz * speed;

    // Wheel rotation
    wheelPitch += -(2 << 4) * speed * deltaT;

    // Current text
    int curr = (int) (abs(pos.z) / 100) + 1;
    if (currText != curr) {
        currText = curr;
        createCommandBuffers();
    }

    // Cars movement
    updateCars(deltaT);
}

void SandroRun::viewHandler(glm::mat4 &ViewProj, glm::mat4 &World) {
    World = glm::translate(glm::mat4(1), pos);
    ViewProj = glm::perspective(FOV_Y, Ar, NEAR_PLANE, FAR_PLANE);
    ViewProj[1][1] *= -1;

    if (holdFire) {
        // First person view
        ViewProj *= glm::rotate(glm::mat4(1.0), (float) (pitch - M_PI / 2.5f), glm::vec3(1, 0, 0)) *
                    glm::rotate(glm::mat4(1.0), yaw, glm::vec3(0, 1, 0)) *
                    glm::rotate(glm::mat4(1.0), -motoRoll / 2.0f, glm::vec3(0, 0, 1)) *
                    glm::translate(glm::mat4(1.0), -pos + glm::vec3((-CAM_HEIGHT - .25f) * sin(-motoRoll),
                                                                    (-CAM_HEIGHT - .25f) * cos(motoRoll) -
                                                                    (CAM_HEIGHT / 2) * sin(motoPitch),
                                                                    -sin(motoPitch)));

        cameraPosition = World * glm::vec4(0, CAM_HEIGHT, 0, 1);
    } else {
        // Third person view
        glm::mat4 rot =
                glm::rotate(glm::mat4(1), -yaw, glm::vec3(0, 1, 0)) *
                glm::rotate(glm::mat4(1), pitch, glm::vec3(1, 0, 0));
        cameraPosition = World * rot * glm::vec4(0, CAM_DIST, 0, 1);
        glm::vec3 a = World * glm::vec4(0, 0, 0, 1) + glm::vec4(0, CAM_HEIGHT, 0, 0);

        ViewProj *= glm::lookAt(cameraPosition, a, glm::vec3(0, 1, 0));
    }
}



void SandroRun::resetGame() {
    // Moto position and orientation
    pos = glm::vec3(0.0f, 0.0f, 0.0f);
    yaw = 0.0f, pitch = M_PI / 2.5f;
    yawNew = 0.0f, pitchNew = M_PI / 2.5f;
    cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    speed = 0;
    motoRoll = 0;
    motoPitch = 0;
    wheelPitch = 0;

    // Game state
    currText = 0;
    gameState = 0;
    splashVisibility = 1.0f;
    frontWorldLimit = -WORLD_LENGTH;
    backWorldLimit = 0;
    dayTime = 0;
    gameOver = false;

    // Cars positions
    initCars();
}
