void SandroRun::roadModel() {

    const int period = 12;

    for (int i = 0; i < PERIODS * period; i++) {
        glm::vec3 surface = glm::vec3({10.0f, 0.0f, (-i * 10.0f) + 5.0f});
        glm::vec3 normal = glm::vec3({0.0f, 1.0f, 0.0f});
        glm::vec2 UV = glm::vec2({0.0f, 0.0f});
        MRoad.vertices.push_back({surface, normal, UV});

        surface = glm::vec3({-10.0f, 0.0f, (-i * 10.0f) + 5.0f});
        normal = glm::vec3({0.0f, 1.0f, 0.0f});
        UV = glm::vec2({0.0f, 1.0f});
        MRoad.vertices.push_back({surface, normal, UV});

        surface = glm::vec3({-10.0f, 0.0f, (-i * 10.0f) - 5.0f});
        normal = glm::vec3({0.0f, 1.0f, 0.0f});
        UV = glm::vec2({1.0f, 1.0f});
        MRoad.vertices.push_back({surface, normal, UV});

        surface = glm::vec3({10.0f, 0.0f, (-i * 10.0f) - 5.0f});
        normal = glm::vec3({0.0f, 1.0f, 0.0f});
        UV = glm::vec2({1.0f, 0.0f});
        MRoad.vertices.push_back({surface, normal, UV});
    }
    for (int i = 0; i < PERIODS * period; ++i) {
        MRoad.indices.push_back(i * 4);
        MRoad.indices.push_back(i * 4 + 2);
        MRoad.indices.push_back(i * 4 + 1);

        MRoad.indices.push_back(i * 4 + 2);
        MRoad.indices.push_back(i * 4);
        MRoad.indices.push_back(i * 4 + 3);
    }

    MRoad.initMesh(this, &VMesh);
    TRoad.init(this, "textures/road.png");
}

void SandroRun::terrainModel() {

    // Right side
    float uPos = 0.0f;
    int inv = -1;
    const int period = 120;

    for (int i = 0; i <= PERIODS * period; i++) {
        for (int j = 0; j <= 60; j++) {

            float x = (float) j + 10.0f;
            float z = (float) -i + 5.0f;
            float y = sin((x - 10.0f) * M_PI / 30.0f) * sin((z + 5.0f) * M_PI / 30.0f) * 5;

            if (y < 0) { y = 0; }

            glm::vec3 surface = glm::vec3({x, y, z});

            glm::vec3 d_surface_x = glm::vec3(
                    {1, (1 / 6.0f) * M_PI * cos(M_PI * (x - 10.0f) / 30) * sin(M_PI * (z + 5.0f) / 30), 0});
            glm::vec3 d_surface_z = glm::vec3(
                    {0, (1 / 6.0f) * M_PI * sin(M_PI * (x - 10.0f) / 30) * cos(M_PI * (z + 5.0f) / 30), 1});

            glm::vec3 normal = cross(d_surface_z, d_surface_x);
            normal = glm::normalize(normal);

            MTerrain.vertices.push_back({surface, normal, {uPos, j / 60.0f}});

        }
        if (i % 60 == 0) {
            inv *= -1;
        }
        uPos += inv * 1.0f / 60.0f;
    }
    for (int i = 0; i < PERIODS * period; ++i) {
        for (int j = 0; j < 60; ++j) {
            MTerrain.indices.push_back(i * 61 + j);
            MTerrain.indices.push_back(i * 61 + j + 1);
            MTerrain.indices.push_back((i + 1) * 61 + j);

            MTerrain.indices.push_back((i + 1) * 61 + j);
            MTerrain.indices.push_back(i * 61 + j + 1);
            MTerrain.indices.push_back((i + 1) * 61 + j + 1);
        }
    }

    uPos = 0.0f;
    inv = -1;

    // Left side
    for (int i = 0; i <= PERIODS * period; i++) {
        for (int j = 0; j <= 60; j++) {

            float x = (float) -j - 10.0f;
            float z = (float) -i + 5.0f;
            float y = sin((x + 10.0f) * M_PI / 30.0f) * sin((z + 5.0f) * M_PI / 30.0f) * 5;

            if (y < 0) { y = 0; }

            glm::vec3 surface = glm::vec3({x, y, z});

            glm::vec3 d_surface_x = glm::vec3(
                    {1, (1 / 6.0f) * M_PI * cos(M_PI * (x + 10.0f) / 30) * sin(M_PI * (z + 5.0f) / 30), 0});
            glm::vec3 d_surface_z = glm::vec3(
                    {0, (1 / 6.0f) * M_PI * sin(M_PI * (x + 10.0f) / 30) * cos(M_PI * (z + 5.0f) / 30), 1});

            glm::vec3 normal = cross(d_surface_z, d_surface_x);
            normal = glm::normalize(normal);

            MTerrain.vertices.push_back({surface, normal, {uPos, j / 60.0f}});
        }
        if (i % 60 == 0) {
            inv *= -1;
        }
        uPos += inv * 1.0f / 60.0f;
    }

    int offset = (PERIODS * period + 1) * 61;

    for (int i = 0; i < PERIODS * period; ++i) {
        for (int j = 0; j < 60; ++j) {
            MTerrain.indices.push_back(offset + i * 61 + j);
            MTerrain.indices.push_back(offset + (i + 1) * 61 + j);
            MTerrain.indices.push_back(offset + i * 61 + j + 1);

            MTerrain.indices.push_back(offset + (i + 1) * 61 + j);
            MTerrain.indices.push_back(offset + (i + 1) * 61 + j + 1);
            MTerrain.indices.push_back(offset + i * 61 + j + 1);

        }
    }

    MTerrain.initMesh(this, &VMesh);
    TTerrain.init(this, "textures/grass.jpg");
}

void SandroRun::skyboxModel() {
    MSkybox.init(this, &VMesh, "models/skyboxCube.obj", OBJ);
    const char *T2fn[] = {
            "textures/skybox/left.png",
            "textures/skybox/right.png",
            "textures/skybox/top.png",
            "textures/skybox/bottom.png",
            "textures/skybox/back.png",
            "textures/skybox/front.png"
    };
    TSkybox.initCubic(this, T2fn);
}

void SandroRun::splashModel() {
    MSplash.vertices = {{{-1.0f, -1.0f}, {0.0f, 0.0f}},
                        {{-1.0f, 1.0f},  {0.0f, 1.0f}},
                        {{1.0f,  -1.0f}, {1.0f, 0.0f}},
                        {{1.0f,  1.0f},  {1.0f, 1.0f}}};
    MSplash.indices = {0, 1, 2, 1, 2, 3};
    MSplash.initMesh(this, &VOverlay);

    TSplash.init(this, "textures/splashScreen.png");
}