void SandroRun::roadModel() {
    for (int i = 0; i < 24; i++) {
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
    for (int i = 0; i < 24; ++i) {
        MRoad.indices.push_back(i * 4);
        MRoad.indices.push_back(i * 4 + 2);
        MRoad.indices.push_back(i * 4 + 1);

        MRoad.indices.push_back(i * 4 + 2);
        MRoad.indices.push_back(i * 4);
        MRoad.indices.push_back(i * 4 + 3);
    }
    TRoad.init(this, "textures/road.png");
}

void SandroRun::terrainModel() {

    // Right side
    for (int i = 0; i <= 240; i++) {
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

            MTerrain.vertices.push_back({surface, normal, {i / 240.0f, j / 60.0f}});
        }
    }
    for (int i = 0; i < 240; ++i) {
        for (int j = 0; j < 60; ++j) {
            MTerrain.indices.push_back(i * 61 + j);
            MTerrain.indices.push_back(i * 61 + j + 1);
            MTerrain.indices.push_back((i + 1) * 61 + j);

            MTerrain.indices.push_back((i + 1) * 61 + j);
            MTerrain.indices.push_back(i * 61 + j + 1);
            MTerrain.indices.push_back((i + 1) * 61 + j + 1);
        }
    }

    // Left side
    for (int i = 0; i <= 240; i++) {
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

            MTerrain.vertices.push_back({surface, normal, {i / 240.0f, j / 60.0f}});
        }
    }

    int offset = 241 * 61;

    for (int i = 0; i < 240; ++i) {
        for (int j = 0; j < 60; ++j) {
            MTerrain.indices.push_back(offset + i * 61 + j);
            MTerrain.indices.push_back(offset + (i + 1) * 61 + j);
            MTerrain.indices.push_back(offset + i * 61 + j + 1);

            MTerrain.indices.push_back(offset + (i + 1) * 61 + j);
            MTerrain.indices.push_back(offset + (i + 1) * 61 + j + 1);
            MTerrain.indices.push_back(offset + i * 61 + j + 1);

        }
    }

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