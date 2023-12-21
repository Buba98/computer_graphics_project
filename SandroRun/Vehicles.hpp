#include <chrono>
#include <thread>

void SandroRun::initCars(){
    for (int m = 0; m < NUM_CAR_MODELS; m++) {
        for (int i = 0; i < NUM_CAR_MODEL_INSTANCES; i++) {
            cars[m][i].pos = glm::vec3(0.0f);
        }
    }
    for (int model = 0; model < NUM_CAR_MODELS; model++) {
        for (int i = 0; i < NUM_CAR_MODEL_INSTANCES; i++) {
            regenerateCar(model, i);
            cars[model][i].length = carsLength[model];
            cars[model][i].width = carsWidth[model];
        }
    }
    for (int model = 0; model < NUM_CAR_MODELS; model++) {
        for (int i = 0; i < NUM_CAR_MODEL_INSTANCES; i++) {
            if (cars[model][i].isGoingForward)
                cars[model][i].pos.z += (float) WORLD_LENGTH * INITIAL_RIGHT_LANES_SHIFTING_FACTOR;
        }
    }
}

void SandroRun::updateCars(double deltaT) {
    for (int model = 0; model < NUM_CAR_MODELS; model++) {
        for (int i = 0; i < NUM_CAR_MODEL_INSTANCES; i++) {
            cars[model][i].pos.z -= cars[model][i].speed * deltaT;
            if (cars[model][i].pos.z > scene.backWorldLimit) {
                regenerateCar(model, i);
            }
        }
    }
}

void SandroRun::regenerateCar(int model, int index) {
    // Selecting random lane
    int lane = (int) (random() % 4);
    switch (lane) {
        case 0:
            cars[model][index].pos.x = LEFT_LANE;
            cars[model][index].isGoingForward = false;
            cars[model][index].speed = LEFT_LANE_CAR_SPEED;
            break;
        case 1:
            cars[model][index].pos.x = CENTER_LEFT_LANE;
            cars[model][index].isGoingForward = false;
            cars[model][index].speed = CENTER_LEFT_LANE_CAR_SPEED;
            break;
        case 2:
            cars[model][index].pos.x = CENTER_RIGHT_LANE;
            cars[model][index].isGoingForward = true;
            cars[model][index].speed = CENTER_RIGHT_LANE_CAR_SPEED;
            break;
        case 3:
            cars[model][index].pos.x = RIGHT_LANE;
            cars[model][index].isGoingForward = true;
            cars[model][index].speed = RIGHT_LANE_CAR_SPEED;
            break;
        default:;
    }

    cars[model][index].pos.z = scene.frontWorldLimit;

    // Preparing data for collision-free placement
    float currentLane = cars[model][index].pos.x;
    float currentZCoord = cars[model][index].pos.z;
    float mostAdvancedZCoord = 0;
    bool otherCarInLaneFound = false;

    // Searching most advanced car in same lane
    for (int m = 0; m < NUM_CAR_MODELS; m++) {
        for (int i = 0; i < NUM_CAR_MODEL_INSTANCES; i++) {
            if (cars[m][i].pos.x == currentLane && cars[m][i].pos.z <= mostAdvancedZCoord && (m != model || i != index)) {
                mostAdvancedZCoord = cars[m][i].pos.z;
                otherCarInLaneFound = true;
            }
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
    cars[model][index].pos.z = currentZCoord;
}

void SandroRun::checkCollisionsWithCars() {
    for (int model = 0; model < NUM_CAR_MODELS; model++) {
        for (int i = 0; i < NUM_CAR_MODEL_INSTANCES; i++) {
            float motoBack = moto.pos.z + MOTO_MODEL_OFFSET;
            float motoFront = motoBack - moto.length;
            float carBack = cars[model][i].isGoingForward ? cars[model][i].pos.z + cars[model][i].length : cars[model][i].pos.z;
            float carFront = carBack - cars[model][i].length;

            if (motoFront <= carBack && motoBack >= carFront) {
                float motoLeft = moto.pos.x - moto.width / 2;
                float motoRight = moto.pos.x + moto.width / 2;
                float carLeft = cars[model][i].pos.x - cars[model][i].width / 2;
                float carRight = cars[model][i].pos.x + cars[model][i].width / 2;

                if (motoLeft <= carRight && motoRight >= carLeft) {
                    std::cout << "Collision with car[" << model << "][" << i << "]" << std::endl;
                    scene.gameOver = true;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    return;
                }
            }
        }
    }
}

void SandroRun::checkCollisionsWithGuardRails() {
    float motoLeft = moto.pos.x - moto.width / 2;
    float motoRight = moto.pos.x + moto.width / 2;

    if (motoLeft <= (- ROAD_WIDTH / 2) || motoRight >= (ROAD_WIDTH / 2)) {
        std::cout << "Collision with guard rail" << std::endl;
        scene.gameOver = true;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }
}