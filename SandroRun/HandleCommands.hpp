#define FOV_Y glm::radians(45.0f)
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f
#define CAM_HEIGHT 1.0f
#define CAM_DIST 5.0f
#define MIN_PITCH glm::radians(10.0f)
#define MAX_PITCH glm::radians(80.0f)
#define ROT_SPEED glm::radians(120.0f)
#define Z_SPEED 3.0f
#define X_SPEED 21.0f
#define MIN_MOTO_ROLL (float) (-M_PI / 4.0f)
#define MAX_MOTO_ROLL (-MIN_MOTO_ROLL)
#define MIN_MOTO_PITCH 0.0f
#define MAX_MOTO_PITCH (float) (M_PI / 4.0f)
#define LAMBDA 10.0f

void SandroRun::handleCommands(glm::mat4 &ViewProj, glm::mat4 &World) {

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
    yaw = yaw * glm::exp(-LAMBDA * deltaT) + yawNew * (1 - glm::exp(-LAMBDA * deltaT)); // Yaw damping

    rollNew += ROT_SPEED * r.z * deltaT;
    roll = roll * glm::exp(-LAMBDA * deltaT) + rollNew * (1 - glm::exp(-LAMBDA * deltaT)); // Roll damping

    motoRoll = motoRoll - m.x * deltaT * 5.0f;
    if (m.x * deltaT == 0.0f) {
        motoRoll = motoRoll * (1 - glm::exp(-200 * deltaT)); // Yaw damping
        if (motoRoll > 0.001 * MIN_MOTO_ROLL && motoRoll < 0.001 * MAX_MOTO_ROLL)
            motoRoll = 0.0f;
    }
    motoRoll = glm::clamp(motoRoll, MIN_MOTO_ROLL, MAX_MOTO_ROLL);
    motoPitch = motoPitch - m.z * deltaT * 2.0f;
    if (m.z * deltaT == 0.0f) {
        motoPitch = motoPitch * (1 - glm::exp(-200 * deltaT)); // Yaw damping
        if (motoPitch < 0.001 * MAX_MOTO_ROLL)
            motoPitch = 0.0f;
    }
    motoPitch = glm::clamp(motoPitch, MIN_MOTO_PITCH, MAX_MOTO_PITCH);

    speed = Z_SPEED * (log(time * .1f + 1) + 1) * .1f;

    pos += ux * X_SPEED * deltaT * sin(-motoRoll);
    pos += uz * speed;

    int curr = (abs(pos.z) / 100) + 1;
    if (currText != curr) {
        currText = curr;
        createCommandBuffers();
    }

    World = glm::translate(glm::mat4(1), pos);

    glm::mat4 View;
    //implement view in and look at
    if (holdFire) {

        View = glm::rotate(glm::mat4(1.0), (float) (pitch - M_PI / 2.5f), glm::vec3(1, 0, 0)) *
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

        View = glm::rotate(glm::mat4(1), -roll, glm::vec3(0, 0, 1)) * glm::lookAt(cameraPosition, a, uy);
    }

    glm::mat4 Proj = glm::perspective(FOV_Y, Ar, NEAR_PLANE, FAR_PLANE);
    Proj[1][1] *= -1;
    ViewProj = Proj * View;
}
