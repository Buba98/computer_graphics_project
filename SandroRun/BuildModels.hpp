void SandroRun::roadModel() {

    const int period = 12;

    for (int i = 0; i < NUM_TERRAIN_REPETITIONS * period; i++) {
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
    for (int i = 0; i < NUM_TERRAIN_REPETITIONS * period; ++i) {
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

    for (int i = 0; i <= NUM_TERRAIN_REPETITIONS * period; i++) {
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
    for (int i = 0; i < NUM_TERRAIN_REPETITIONS * period; ++i) {
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
    for (int i = 0; i <= NUM_TERRAIN_REPETITIONS * period; i++) {
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

    int offset = (NUM_TERRAIN_REPETITIONS * period + 1) * 61;

    for (int i = 0; i < NUM_TERRAIN_REPETITIONS * period; ++i) {
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

    const char *T2fn_day[] = {
            "textures/skybox/day/left.png",
            "textures/skybox/day/right.png",
            "textures/skybox/day/top.png",
            "textures/skybox/day/bottom.png",
            "textures/skybox/day/back.png",
            "textures/skybox/day/front.png"
    };
    TSkybox[0].initCubic(this, T2fn_day);

    const char *T2fn_sunset[] = {
            "textures/skybox/sunset/left.png",
            "textures/skybox/sunset/right.png",
            "textures/skybox/sunset/top.png",
            "textures/skybox/sunset/bottom.png",
            "textures/skybox/sunset/back.png",
            "textures/skybox/sunset/front.png"
    };
    TSkybox[1].initCubic(this, T2fn_sunset);

    const char *T2fn_night[] = {
            "textures/skybox/night/left.png",
            "textures/skybox/night/right.png",
            "textures/skybox/night/top.png",
            "textures/skybox/night/bottom.png",
            "textures/skybox/night/back.png",
            "textures/skybox/night/front.png"
    };
    TSkybox[2].initCubic(this, T2fn_night);
}

void SandroRun::splashModel() {
    MSplash.vertices = {{{-1.0f, -1.0f}, {0.0f, 0.0f}},
                        {{-1.0f, 1.0f},  {0.0f, 1.0f}},
                        {{1.0f,  -1.0f}, {1.0f, 0.0f}},
                        {{1.0f,  1.0f},  {1.0f, 1.0f}}};
    MSplash.indices = {0, 1, 2, 1, 2, 3};
    MSplash.initMesh(this, &VOverlay);

    TSplashStart.init(this, "textures/splash_screens/splashScreenStart.png");
    TSplashEnd.init(this, "textures/splash_screens/splashScreenEnd.png");
}