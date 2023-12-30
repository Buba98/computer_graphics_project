#include <chrono>
#include <thread>

void SandroRun::initCars() {
    for (Car &car: cars) {
        car.pos = glm::vec3(0.0f);
    }
    for (int model = 0; model < NUM_CAR_MODELS; model++) {
        regenerateCar(cars[model]);
        cars[model].length = carsLength[model];
        cars[model].width = carsWidth[model];
    }
    for (Car &car: cars) {
        if (car.isGoingForward) {
            car.pos.z += (float) WORLD_LENGTH * INITIAL_RIGHT_LANES_SHIFTING_FACTOR;
        }
    }
}

void SandroRun::updateCars(float deltaT) {
    for (Car &car: cars) {
        car.pos.z -= car.speed * deltaT;
        if (car.pos.z > scene.backWorldLimit) {
            regenerateCar(car);

        }
    }
}

void SandroRun::regenerateCar(Car &car) {
    // Selecting random lane
    int lane = (int) (random() % 4);
    switch (lane) {
        case 0:
            car.pos.x = LEFT_LANE;
            car.isGoingForward = false;
            car.speed = LEFT_LANE_CAR_SPEED;
            break;
        case 1:
            car.pos.x = CENTER_LEFT_LANE;
            car.isGoingForward = false;
            car.speed = CENTER_LEFT_LANE_CAR_SPEED;
            break;
        case 2:
            car.pos.x = CENTER_RIGHT_LANE;
            car.isGoingForward = true;
            car.speed = CENTER_RIGHT_LANE_CAR_SPEED;
            break;
        case 3:
            car.pos.x = RIGHT_LANE;
            car.isGoingForward = true;
            car.speed = RIGHT_LANE_CAR_SPEED;
            break;
        default:;
    }

    car.pos.z = scene.frontWorldLimit;

    // Preparing data for collision-free placement
    float currentLane = car.pos.x;
    float currentZCoord = car.pos.z;
    float mostAdvancedZCoord = 0;
    bool otherCarInLaneFound = false;

    // Searching most advanced car in same lane
    for (Car &otherCar: cars) {
        if (otherCar.pos.x == currentLane && otherCar.pos.z <= mostAdvancedZCoord && &car != &otherCar) {
            mostAdvancedZCoord = otherCar.pos.z;
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
    car.pos.z = currentZCoord;
}

bool SandroRun::checkCollisionsWithCars() {
    for (Car &car: cars) {
        float motoBack = moto.pos.z + MOTO_MODEL_OFFSET;
        float motoFront = motoBack - MOTO_LENGTH * cos(moto.pitch);
        float carBack = car.isGoingForward ? car.pos.z + car.length : car.pos.z;
        float carFront = carBack - car.length;

        if (motoFront <= carBack && motoBack >= carFront) {
            float motoLateralInclinationCoord = moto.pos.x + MOTO_HEIGHT * sin(-moto.roll);
            float motoLeft = std::min(moto.pos.x - MOTO_WIDTH / 2, motoLateralInclinationCoord);
            float motoRight = std::max(moto.pos.x + MOTO_WIDTH / 2, motoLateralInclinationCoord);
            float carLeft = car.pos.x - car.width / 2;
            float carRight = car.pos.x + car.width / 2;

            if (motoLeft <= carRight && motoRight >= carLeft) {
                std::cout << "Collision with car" << std::endl;
                return true;
            }
        }
    }
    return false;
}

bool SandroRun::checkCollisionsWithGuardRails() const {
    float motoLateralInclinationCoord = moto.pos.x + MOTO_HEIGHT * sin(-moto.roll);
    float motoLeft = std::min(moto.pos.x - MOTO_WIDTH / 2, motoLateralInclinationCoord);
    float motoRight = std::max(moto.pos.x + MOTO_WIDTH / 2, motoLateralInclinationCoord);

    if (motoLeft <= (-ROAD_WIDTH / 2 + GUARD_RAIL_WIDTH) || motoRight >= (ROAD_WIDTH / 2 - GUARD_RAIL_WIDTH)) {
        std::cout << "Collision with guard rail" << std::endl;
        return true;
    }
    return false;
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

void SandroRun::gameOverAnimation(float deltaT) {
    if (moto.roll > 0) {
        moto.roll = std::clamp(moto.roll + deltaT, -M_PI_2f, M_PI_2f);
    } else {
        moto.roll = std::clamp(moto.roll - deltaT, -M_PI_2f, M_PI_2f);
    }

    if (abs(moto.roll) >= (float) M_PI_2) {
        std::cout << "Game over" << std::endl;
        scene.gameState = GAME_OVER_SCREEN;
        refreshAudio();
    }
}
