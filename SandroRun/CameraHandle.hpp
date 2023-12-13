void SandroRun::updateCameraPosition(glm::mat4 &ViewProj, glm::mat4 &World, glm::vec3 &cameraPosition) {
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