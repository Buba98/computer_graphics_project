void SandroRun::handleCommands(glm::mat4 &ViewProj, glm::mat4 &World) {

    const float ROAD_WIDTH = 8.68f;
    const float FOV_Y = glm::radians(45.0f);
    const float NEAR_PLANE = 0.1f;
    const float FAR_PLANE = 100.0f;

    const float CAM_HEIGHT = 1.0f;
    const float CAM_DIST = 5.0f;

    const float MIN_PITCH = glm::radians(10.0f);
    const float MAX_PITCH = glm::radians(80.0f);

    const float ROT_SPEED = glm::radians(120.0f);
    const float MOV_SPEED = 3.0f;

    const float MIN_MOTO_ROLL = (float) (-M_PI / 4.0f);
    const float MAX_MOTO_ROLL = (float) (M_PI / 4.0f);

    const float MIN_MOTO_PITCH = 0;
    const float MAX_MOTO_PITCH = (float) (M_PI / 4.0f);

    float deltaT;
    float time;
    auto m = glm::vec3(0.0f), r = glm::vec3(0.0f);

    bool fire = false;
    getSixAxis(deltaT, m, r, fire, time);

    bool handleFire = (wasFire && (!fire));
    wasFire = fire;
    if (handleFire)
        holdFire = !holdFire;

    const float LAMBDA = 10.0f;

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

    pos += ux * MOV_SPEED * deltaT * -(4 * motoRoll * motoRoll * (motoRoll >= 0 ? 1 : -1));
    speed = (log(time * .1f + 1) + 1) * .1f;
    pos += uz * MOV_SPEED * speed;

    motoPitch = motoPitch - m.z * deltaT * 2.0f;
    if (m.z * deltaT == 0.0f) {
        motoPitch = motoPitch * (1 - glm::exp(-200 * deltaT)); // Yaw damping
        if (motoPitch < 0.001 * MAX_MOTO_ROLL)
            motoPitch = 0.0f;
    }
    motoPitch = glm::clamp(motoPitch, MIN_MOTO_PITCH, MAX_MOTO_PITCH);

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
