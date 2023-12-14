void SandroRun::updateCameraPosition(glm::mat4 &ViewProj, glm::mat4 &World, glm::vec3 &cameraPosition) {
    const float ROAD_WIDTH = 8.68f;
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
    auto m = glm::vec3(0.0f), r = glm::vec3(0.0f);

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

    if(abs(pos.x) < ROAD_WIDTH || pos.x*m.x < 0)
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

void SandroRun::getSixAxis(float &deltaT, glm::vec3 &m, glm::vec3 &r, bool &fire) {
    const float SPEED_INCREASE = 0.001f;
    const float MAX_SPEED = 20.0f;
    static auto startTime = std::chrono::high_resolution_clock::now();
    static float lastTime = 0.0f;

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    deltaT = time - lastTime;
    lastTime = time;

    static double old_xpos = 0, old_ypos = 0;
    static float old_mz = 0;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float m_dx = xpos - old_xpos;
    float m_dy = ypos - old_ypos;
    old_xpos = xpos;
    old_ypos = ypos;

    const float MOUSE_RES = 10.0f;
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        r.y = -m_dx / MOUSE_RES;
        r.x = -m_dy / MOUSE_RES;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT)) {
        r.y = -1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
        r.y = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP)) {
        r.x = -1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN)) {
        r.x = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q)) {
        r.z = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_E)) {
        r.z = -1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_A)) {
        m.x = -1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        m.x = 1.0f;
    }
    if (old_mz < MAX_SPEED) old_mz += SPEED_INCREASE;
    m.z = old_mz;
    if (glfwGetKey(window, GLFW_KEY_R)) {
        m.y = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_F)) {
        m.y = -1.0f;
    }

    fire = (glfwGetKey(window, GLFW_KEY_SPACE) | glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) == GLFW_PRESS;
    handleGamePad(GLFW_JOYSTICK_1, m, r, fire);
    handleGamePad(GLFW_JOYSTICK_2, m, r, fire);
    handleGamePad(GLFW_JOYSTICK_3, m, r, fire);
    handleGamePad(GLFW_JOYSTICK_4, m, r, fire);
}
