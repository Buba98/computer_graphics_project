void Assignment14::createCubeMesh(std::vector <Vertex> &vDef, std::vector <uint32_t> &vIdx) {
    // The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
    // The primitive built here is a cube centered in the origin, whose edge length is 2.

    // Fill array vPos with the positions of the vertices of the mesh
    vDef.push_back({{-1.0f, -1.0f, 1.0f},
                    {0.0f,  0.0f,  1.0f}});    // vertex 0 - Position and Normal
    vDef.push_back({{1.0f, -1.0f, 1.0f},
                    {0.0f, 0.0f,  1.0f}});    // vertex 1 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, 1.0f},
                    {0.0f,  0.0f, 1.0f}});    // vertex 2 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, 1.0f},
                    {0.0f, 0.0f, 1.0f}});  // vertex 3 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(0);
    vIdx.push_back(1);
    vIdx.push_back(2);    // First triangle
    vIdx.push_back(1);
    vIdx.push_back(2);
    vIdx.push_back(3);    // Second triangle

    // Fill array vPos with the positions of the vertices of the mesh
    vDef.push_back({{-1.0f, -1.0f, -1.0f},
                    {0.0f,  0.0f,  -1.0f}});    // vertex 4 - Position and Normal
    vDef.push_back({{1.0f, -1.0f, -1.0f},
                    {0.0f, 0.0f,  -1.0f}});    // vertex 5 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, -1.0f},
                    {0.0f,  0.0f, -1.0f}});    // vertex 6 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, -1.0f},
                    {0.0f, 0.0f, -1.0f}});  // vertex 7 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(4);
    vIdx.push_back(5);
    vIdx.push_back(6);    // First triangle
    vIdx.push_back(5);
    vIdx.push_back(6);
    vIdx.push_back(7);    // Second triangle

    // Fill array vPos with the positions of the vertices of the mesh
    vDef.push_back({{1.0f, -1.0f, -1.0f},
                    {1.0f, 0.0f,  0.0f}});    // vertex 8 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, -1.0f},
                    {1.0f, 0.0f, 0.0f}});    // vertex 9 - Position and Normal
    vDef.push_back({{1.0f, -1.0f, 1.0f},
                    {1.0f, 0.0f,  0.0f}});    // vertex 10 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, 1.0f},
                    {1.0f, 0.0f, 0.0f}});  // vertex 11 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(8);
    vIdx.push_back(9);
    vIdx.push_back(10);    // First triangle
    vIdx.push_back(9);
    vIdx.push_back(10);
    vIdx.push_back(11);    // Second triangle

    // Fill array vPos with the positions of the vertices of the mesh
    vDef.push_back({{-1.0f, -1.0f, -1.0f},
                    {-1.0f, 0.0f,  0.0f}});    // vertex 12 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, -1.0f},
                    {-1.0f, 0.0f, 0.0f}});    // vertex 13 - Position and Normal
    vDef.push_back({{-1.0f, -1.0f, 1.0f},
                    {-1.0f, 0.0f,  0.0f}});    // vertex 14 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, 1.0f},
                    {-1.0f, 0.0f, 0.0f}});  // vertex 15 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(12);
    vIdx.push_back(13);
    vIdx.push_back(14);    // First triangle
    vIdx.push_back(13);
    vIdx.push_back(14);
    vIdx.push_back(15);    // Second triangle

    // Fill array vPos with the positions of the vertices of the mesh
    vDef.push_back({{-1.0f, 1.0f, -1.0f},
                    {0.0f,  1.0f, 0.0f}});    // vertex 16 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, -1.0f},
                    {0.0f, 1.0f, 0.0f}});    // vertex 17 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, 1.0f},
                    {0.0f,  1.0f, 0.0f}});    // vertex 18 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, 1.0f},
                    {0.0f, 1.0f, 0.0f}});  // vertex 19 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(16);
    vIdx.push_back(17);
    vIdx.push_back(18);    // First triangle
    vIdx.push_back(17);
    vIdx.push_back(18);
    vIdx.push_back(19);    // Second triangle

    // Fill array vPos with the positions of the vertices of the mesh
    vDef.push_back({{-1.0f, -1.0f, -1.0f},
                    {0.0f,  -1.0f, 0.0f}});    // vertex 20 - Position and Normal
    vDef.push_back({{1.0f, -1.0f, -1.0f},
                    {0.0f, -1.0f, 0.0f}});    // vertex 21 - Position and Normal
    vDef.push_back({{-1.0f, -1.0f, 1.0f},
                    {0.0f,  -1.0f, 0.0f}});    // vertex 22 - Position and Normal
    vDef.push_back({{1.0f, -1.0f, 1.0f},
                    {0.0f, -1.0f, 0.0f}});  // vertex 23 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(20);
    vIdx.push_back(21);
    vIdx.push_back(22);    // First triangle
    vIdx.push_back(21);
    vIdx.push_back(22);
    vIdx.push_back(23);    // Second triangle

}

void Assignment14::createFunctionMesh(std::vector <Vertex> &vDef, std::vector <uint32_t> &vIdx) {
    // The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
    // The procedures also fill the array vIdx with the indices of the vertices of the triangles
    // The primitive built here is the surface y = sin(x) * cos(z) with -3 <= x <= 3 and -3 <= z <= 3.

    // Fill array vPos with the positions of the vertices of the mesh


    for (float x = -3.0f; x <= 3.0f; x += 0.1f) {
        for (float z = -3.0f; z <= 3.0f; z += 0.1f) {

            glm::vec3 surface = glm::vec3({x, sin(x) * cos(z), z});
            glm::vec3 d_surface_x = glm::vec3({1, cos(x) * cos(z), 0});
            glm::vec3 d_surface_z = glm::vec3({0, -sin(x) * sin(z), 1});

            glm::vec3 normal = cross(d_surface_z, d_surface_x);

            normal = glm::normalize(normal);

            vDef.push_back({surface, normal});    // vertex 0 - Position and Normal
        }
    }

    // Fill the array vIdx with the indices of the vertices of the triangles

    for (int i = 0; i < 60; i++) {
        for (int j = 0; j < 60; j++) {
            vIdx.push_back(i * 61 + j);
            vIdx.push_back(i * 61 + j + 1);
            vIdx.push_back((i + 1) * 61 + j);    // First triangle
            vIdx.push_back(i * 61 + j + 1);
            vIdx.push_back((i + 1) * 61 + j);
            vIdx.push_back((i + 1) * 61 + j + 1);    // Second triangle
        }
    }
}

void Assignment14::createCylinderMesh(std::vector <Vertex> &vDef, std::vector <uint32_t> &vIdx) {
    // The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
    // The procedures also fill the array vIdx with the indices of the vertices of the triangles
    // The primitive built here is a cylinder, with radius 1, and height 2, centered in the origin.

    // Fill array vPos with the positions of the vertices of the mesh

    float increment = 0.01f;
    int runs = (int) (2.0f / increment);

    glm::vec3 norm_top = glm::vec3({0, 1, 0});
    glm::vec3 norm_bottom = glm::vec3({0, -1, 0});

    Vertex top = {{0.0f, 1.0f, 0.0f}, norm_top};
    Vertex bottom = {{0.0f, -1.0f, 0.0f}, norm_bottom};

    vDef.push_back(bottom);
    vDef.push_back(top);

    for (float y = -1.0f; y <= 1.0f; y += 2.0f) {
        for (float angle = 0.0f; angle <= 2.0f * M_PI; angle += increment * M_PI) {
            glm::vec3 surface = glm::vec3({cos(angle), y, sin(angle)});
            vDef.push_back({surface, y > 0 ? norm_top : norm_bottom});    // vertex 0 - Position and Normal
        }
    }

    // Fill the array vIdx with the indices of the vertices of the triangles

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < runs - 1; j += 1) {
            vIdx.push_back(i);
            vIdx.push_back(2 + i * runs + j);
            vIdx.push_back(2 + i * runs + j + 1);
        }

        vIdx.push_back(i);
        vIdx.push_back(2 + i * runs + runs - 1);
        vIdx.push_back(2 + i * runs);
    }

    for (float angle = 0.0f; angle <= 2.0f * M_PI; angle += increment * M_PI) {
        glm::vec3 surface_bottom = glm::vec3({cos(angle), -1, sin(angle)});
        glm::vec3 surface_top = glm::vec3({cos(angle), 1, sin(angle)});

        glm::vec3 normal = glm::vec3({cos(angle), 0, sin(angle)});

        // normal = glm::normalize(normal);

        vDef.push_back({surface_bottom, normal});
        vDef.push_back({surface_top, normal});
    }

    for (int i = 2 * runs + 2; i < 2 + 4 * runs - 2; i+=2) {
        vIdx.push_back(i);
        vIdx.push_back(i + 1);
        vIdx.push_back(i + 2);

        vIdx.push_back(i + 1);
        vIdx.push_back(i + 2);
        vIdx.push_back(i + 3);
    }

    vIdx.push_back(2 + 4 * runs - 2);
    vIdx.push_back(2 + 4 * runs - 1);
    vIdx.push_back(2 + 2 * runs);

    vIdx.push_back(2 + 4 * runs - 1);
    vIdx.push_back(2 + 2 * runs);
    vIdx.push_back(2 + 2 * runs + 1);
}
