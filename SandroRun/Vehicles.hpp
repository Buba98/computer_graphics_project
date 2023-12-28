#include <chrono>
#include <thread>

void SandroRun::initCars() {
    for (int m = 0; m < NUM_CAR_MODELS; m++) {
        cars[m].pos = glm::vec3(0.0f);
    }
    for (int model = 0; model < NUM_CAR_MODELS; model++) {
        regenerateCar(model);
        cars[model].length = carsLength[model];
        cars[model].width = carsWidth[model];

    }
    for (int model = 0; model < NUM_CAR_MODELS; model++) {
        if (cars[model].isGoingForward) {
            cars[model].pos.z += (float) WORLD_LENGTH * INITIAL_RIGHT_LANES_SHIFTING_FACTOR;
        }
    }
}

void SandroRun::updateCars(float deltaT) {
    for (int model = 0; model < NUM_CAR_MODELS; model++) {
        cars[model].pos.z -= cars[model].speed * deltaT;
        if (cars[model].pos.z > scene.backWorldLimit) {
            regenerateCar(model);

        }
    }
}

void SandroRun::regenerateCar(int model) {
    // Selecting random lane
    int lane = (int) (random() % 4);
    switch (lane) {
        case 0:
            cars[model].pos.x = LEFT_LANE;
            cars[model].isGoingForward = false;
            cars[model].speed = LEFT_LANE_CAR_SPEED;
            break;
        case 1:
            cars[model].pos.x = CENTER_LEFT_LANE;
            cars[model].isGoingForward = false;
            cars[model].speed = CENTER_LEFT_LANE_CAR_SPEED;
            break;
        case 2:
            cars[model].pos.x = CENTER_RIGHT_LANE;
            cars[model].isGoingForward = true;
            cars[model].speed = CENTER_RIGHT_LANE_CAR_SPEED;
            break;
        case 3:
            cars[model].pos.x = RIGHT_LANE;
            cars[model].isGoingForward = true;
            cars[model].speed = RIGHT_LANE_CAR_SPEED;
            break;
        default:;
    }

    cars[model].pos.z = scene.frontWorldLimit;

    // Preparing data for collision-free placement
    float currentLane = cars[model].pos.x;
    float currentZCoord = cars[model].pos.z;
    float mostAdvancedZCoord = 0;
    bool otherCarInLaneFound = false;

    // Searching most advanced car in same lane
    for (int m = 0; m < NUM_CAR_MODELS; m++) {
        if (cars[m].pos.x == currentLane && cars[m].pos.z <= mostAdvancedZCoord &&
            (m != model)) {
            mostAdvancedZCoord = cars[m].pos.z;
            otherCarInLaneFound = true;
        }
    }


    if (!otherCarInLaneFound) {
        return;
    }

// Adjusting position if most advanced car is too close (avoiding initial overlap)
    if (currentZCoord >= mostAdvancedZCoord) {
        currentZCoord = mostAdvancedZCoord - AVOID_INITIAL_OVERLAP_OFFSET;
    }
    if (mostAdvancedZCoord - currentZCoord < AVOID_INITIAL_OVERLAP_OFFSET) {
        currentZCoord -= AVOID_INITIAL_OVERLAP_OFFSET;
    }

// Collision-free corrections
    cars[model].pos.
            z = currentZCoord;
}

void SandroRun::checkCollisionsWithCars() {
    for (int model = 0; model < NUM_CAR_MODELS; model++) {
        float motoBack = moto.pos.z + MOTO_MODEL_OFFSET;
        float motoFront = motoBack - MOTO_LENGTH * cos(moto.pitch);
        float carBack = cars[model].isGoingForward ? cars[model].pos.z + cars[model].length
                                                   : cars[model].pos.z;
        float carFront = carBack - cars[model].length;

        if (motoFront <= carBack && motoBack >= carFront) {
            float motoLateralInclinationCoord = moto.pos.x + MOTO_HEIGHT * sin(-moto.roll);
            float motoLeft = std::min(moto.pos.x - MOTO_WIDTH / 2, motoLateralInclinationCoord);
            float motoRight = std::max(moto.pos.x + MOTO_WIDTH / 2, motoLateralInclinationCoord);
            float carLeft = cars[model].pos.x - cars[model].width / 2;
            float carRight = cars[model].pos.x + cars[model].width / 2;

            if (motoLeft <= carRight && motoRight >= carLeft) {
                std::cout << "Collision with car[" << model << "]" << std::endl;
                scene.gameOver = true;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                return;
            }
        }
    }
}

void SandroRun::checkCollisionsWithGuardRails() {
    float motoLateralInclinationCoord = moto.pos.x + MOTO_HEIGHT * sin(-moto.roll);
    float motoLeft = std::min(moto.pos.x - MOTO_WIDTH / 2, motoLateralInclinationCoord);
    float motoRight = std::max(moto.pos.x + MOTO_WIDTH / 2, motoLateralInclinationCoord);

    if (motoLeft <= (-ROAD_WIDTH / 2 + GUARD_RAIL_WIDTH) || motoRight >= (ROAD_WIDTH / 2 - GUARD_RAIL_WIDTH)) {
        std::cout << "Collision with guard rail" << std::endl;
        scene.gameOver = true;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }
}

void SandroRun::updateMoto(float deltaT, float time, glm::vec3 m, glm::vec3 ux, glm::vec3 uz) {
    // Moto rotation
    moto.roll = moto.roll - m.x * deltaT * 5.0f;
    if (m.x * deltaT == 0.0f) {
        moto.roll *= (1 - glm::exp(-MOTO_ROLL_SPEED * deltaT)); // motoRoll damping
        if (moto.roll > 0.001 * MIN_MOTO_ROLL && moto.roll < 0.001 * MAX_MOTO_ROLL)
            moto.roll = 0.0f;
    }
    moto.roll = glm::clamp(moto.roll, MIN_MOTO_ROLL, MAX_MOTO_ROLL);

    moto.pitch = moto.pitch - m.z * deltaT * 2.0f;
    if (m.z * deltaT == 0.0f) {
        moto.pitch *= (1 - glm::exp(-MOTO_PITCH_SPEED * deltaT)); // motoPitch damping
        if (moto.pitch < 0.001 * MAX_MOTO_PITCH)
            moto.pitch = 0.0f;
    }
    moto.pitch = glm::clamp(moto.pitch, MIN_MOTO_PITCH, MAX_MOTO_PITCH);

    // Moto position
    moto.speed = Z_SPEED * (((time - scene.startTime) * .1f + 1) + 1) * .1f;
    moto.pos += ux * X_SPEED * deltaT * sin(-moto.roll);
    moto.pos += uz * moto.speed;

    // Wheel rotation
    moto.wheelPitch += -(2 << 4) * moto.speed * deltaT;
}
