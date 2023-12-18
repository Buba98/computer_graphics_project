#include "Parameters.hpp"

void SandroRun::controller() {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    float deltaT;
    float time;
    bool fire = false;
    glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);

    getSixAxis(deltaT, m, r, fire, time);

    bool handleFire = (!(wasFire) && fire);
    if (handleFire)
        holdFire = !holdFire && gameState;
    wasFire = fire;

    if (gameState == 0) {
        if (fire) {
            gameState = 1;
            currText = 1;
            createCommandBuffers();
        }
        return;
    }

    if (splashVisibility != 0.0f) {
        splashVisibility = glm::clamp(splashVisibility - deltaT, 0.0f, 1.0f);
    }

    glm::vec3 ux = glm::vec3(1, 0, 0);
    glm::vec3 uy = glm::vec3(0, 1, 0);
    glm::vec3 uz = glm::vec3(0, 0, -1);

    pitchNew += ROT_SPEED * r.x * deltaT;
    pitchNew = glm::clamp(pitchNew, MIN_PITCH, MAX_PITCH);
    pitch = pitch * glm::exp(-LAMBDA * deltaT) + pitchNew * (1 - glm::exp(-LAMBDA * deltaT)); // Pitch damping
    pitch = glm::clamp(pitch, MIN_PITCH, MAX_PITCH);

    yawNew += ROT_SPEED * r.y * deltaT;
    yawNew = glm::clamp(yawNew, MIN_YAW, MAX_YAW);
    yaw = yaw * glm::exp(-LAMBDA * deltaT) + yawNew * (1 - glm::exp(-LAMBDA * deltaT)); // Yaw damping
    yaw = glm::clamp(yaw, MIN_YAW, MAX_YAW);

    rollNew += ROT_SPEED * r.z * deltaT;
    roll = roll * glm::exp(-LAMBDA * deltaT) + rollNew * (1 - glm::exp(-LAMBDA * deltaT)); // Roll damping

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

    speed = Z_SPEED * (log(time * .1f + 1) + 1) * .1f;

    pos += ux * X_SPEED * deltaT * sin(-motoRoll);
    pos += uz * speed;

    wheelPitch += -(2 << 4) * speed * deltaT; // Pitch damping

    int curr = (abs(pos.z) / 100) + 1;
    if (currText != curr) {
        currText = curr;
        createCommandBuffers();
    }

    // Cars positions update
    for (int model = 0; model < NUM_CAR_MODELS; model++) {
        for (int i = 0; i < NUM_CAR_MODEL_INSTANCES; i++) {
            cars[model][i].pos.z -= cars[model][i].velocity * deltaT;
            if (cars[model][i].pos.z > backWorldLimit) {
                regenerateCar(model, i);
            }
        }
    }
}

void SandroRun::viewHandler(glm::mat4 &ViewProj, glm::mat4 &World) {

    World = glm::translate(glm::mat4(1), pos);
    ViewProj = glm::perspective(FOV_Y, Ar, NEAR_PLANE, FAR_PLANE);
    ViewProj[1][1] *= -1;

    if (holdFire) {

        ViewProj *= glm::rotate(glm::mat4(1.0), (float) (pitch - M_PI / 2.5f), glm::vec3(1, 0, 0)) *
                    glm::rotate(glm::mat4(1.0), yaw, glm::vec3(0, 1, 0)) *
                    glm::rotate(glm::mat4(1.0), -motoRoll / 2.0f, glm::vec3(0, 0, 1)) *
                    glm::translate(glm::mat4(1.0), -pos + glm::vec3((-CAM_HEIGHT - .25f) * sin(-motoRoll),
                                                                    (-CAM_HEIGHT - .25f) * cos(motoRoll) -
                                                                    (CAM_HEIGHT / 2) * sin(motoPitch),
                                                                    -sin(motoPitch)));

        cameraPosition = World * glm::vec4(0, CAM_HEIGHT, 0, 1);
    } else {
        glm::mat4 rot =
                glm::rotate(glm::mat4(1), -yaw, glm::vec3(0, 1, 0)) *
                glm::rotate(glm::mat4(1), pitch, glm::vec3(1, 0, 0));
        cameraPosition = World * rot * glm::vec4(0, CAM_DIST, 0, 1);
        glm::vec3 a = World * glm::vec4(0, 0, 0, 1) + glm::vec4(0, CAM_HEIGHT, 0, 0);

        ViewProj *= glm::rotate(glm::mat4(1), -roll, glm::vec3(0, 0, 1)) *
                    glm::lookAt(cameraPosition, a, glm::vec3(0, 1, 0));
    }
}

void SandroRun::regenerateCar(int model, int index) {
    // Selecting random lane
    int lane = (int) (random() % 4);
    switch (lane) {
        case 0:
            cars[model][index].pos.x = LEFT_LANE;
            cars[model][index].isGoingForward = false;
            break;
        case 1:
            cars[model][index].pos.x = CENTER_LEFT_LANE;
            cars[model][index].isGoingForward = false;
            break;
        case 2:
            cars[model][index].pos.x = CENTER_RIGHT_LANE;
            cars[model][index].isGoingForward = true;
            break;
        case 3:
            cars[model][index].pos.x = RIGHT_LANE;
            cars[model][index].isGoingForward = true;
            break;
        default:;
    }

    // Generating random position and velocity
    cars[model][index].pos.z = frontWorldLimit;
    cars[model][index].pos.z -= (float) (random() % 11) / 10 * TERRAIN_LENGTH;
    cars[model][index].velocity = (float) (random() % (int) (MAX_CAR_SPEED - MIN_CAR_SPEED) + MIN_CAR_SPEED);
    cars[model][index].velocity *= cars[model][index].isGoingForward ? 1 : -1;

    // Preparing data for collision-free corrections
    float currentLane = cars[model][index].pos.x;
    float currentZCoord = cars[model][index].pos.z;
    float currentVelocity = cars[model][index].velocity;
    bool currentlyGoingForward = cars[model][index].isGoingForward;
    float mostAdvancedZCoord = 0;
    float mostAdvancedVelocity = 0;
    bool otherCarInLaneFound = false;

    // Searching most advanced car in same lane
    for (int m = 0; m < NUM_CAR_MODELS; m++) {
        for (int i = 0; i < NUM_CAR_MODEL_INSTANCES; i++) {
            if (cars[m][i].pos.x == currentLane && cars[m][i].pos.z < mostAdvancedZCoord && m != model && i != index) {
                mostAdvancedZCoord = cars[m][i].pos.z;
                mostAdvancedVelocity = cars[m][i].velocity;
                otherCarInLaneFound = true;
            }
        }
    }

    if (!otherCarInLaneFound) {
//        std::cout << "Regenerated >> Car[" << model << "][" << index << "]: (x = " << cars[model][index].pos.x << ", y = " << cars[model][index].pos.y << ", z = " << cars[model][index].pos.z << ") "
//                  << ", velocity = " << cars[model][index].velocity << std::endl;
        return;
    }

    // Adjusting position if most advanced car is too close (avoiding initial overlap)
    if (currentZCoord > mostAdvancedZCoord)
        currentZCoord = mostAdvancedZCoord - AVOID_INITIAL_OVERLAP_OFFSET;
    if (mostAdvancedZCoord - currentZCoord < AVOID_INITIAL_OVERLAP_OFFSET)
        currentZCoord -= AVOID_INITIAL_OVERLAP_OFFSET;

    // Correcting velocity if current car is going forward too slowly or going backward too fast
    if (currentVelocity < mostAdvancedVelocity) {
        float idealCollisionZCoord = currentZCoord;
        idealCollisionZCoord -= currentlyGoingForward ? FRONT_COLLISION_Z_COORD_OFFSET : BACK_COLLISION_Z_COORD_OFFSET;
        float minIdealVelocity = mostAdvancedVelocity * (idealCollisionZCoord - currentZCoord) /
                                 (idealCollisionZCoord - mostAdvancedZCoord);
        if (currentVelocity < minIdealVelocity)
            currentVelocity = minIdealVelocity;
    }

    // Collision-free corrections
    cars[model][index].pos.z = currentZCoord;
    cars[model][index].velocity = currentVelocity;

//    std::cout << "Regenerated >> Car[" << model << "][" << index << "]: (x = " << cars[model][index].pos.x << ", y = " << cars[model][index].pos.y << ", z = " << cars[model][index].pos.z << ") "
//              << ", velocity = " << cars[model][index].velocity << std::endl;
}