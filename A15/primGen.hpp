
void Assignment15::createBoxMesh(std::vector<Vertex> &vDef,
                                 std::vector<uint32_t> &vIdx) {
    // The primitive built here is a box centered in the origin, with
    // proportions respecting the texture.

    // The procedure fills array vPos with the positions of the vertices and of
    // the normal vectors of the mesh
    const float texture_width = 1024;
    const float texture_height = 512;

    const float box_width = 350;
    const float box_height = 324;
    const float box_depth = 94;

    const float scaled_box_width = box_width / box_depth;
    const float scaled_box_height = box_height / box_depth;
    const float scaled_box_depth = box_depth / box_depth;

    const glm::vec3 scale_factor = {scaled_box_width / 2, scaled_box_height / 2,
                                    scaled_box_depth / 2};

    glm::vec3 positions[8] = {
        {-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1},
        {1, -1, -1},  {1, -1, 1},  {1, 1, -1},  {1, 1, 1},
    };

    glm::vec3 normals[6] = {
        {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1},
    };

    glm::vec2 UVs[24] = {
        // Left
        {0.0f, (box_depth + box_height) / texture_height},
        {box_depth / texture_width, (box_depth + box_height) / texture_height},
        {0.0f, box_depth / texture_height},
        {box_depth / texture_width, box_depth / texture_height},
        // Right
        {(2 * box_depth + box_width) / texture_width,
         (box_depth + box_height) / texture_height},
        {(box_depth + box_width) / texture_width,
         (box_depth + box_height) / texture_height},
        {(2 * box_depth + box_width) / texture_width,
         box_depth / texture_height},
        {(box_depth + box_width) / texture_width, box_depth / texture_height},
        // Bottom
        {box_depth / texture_width,
         (2 * box_depth + box_height) / texture_height},
        {box_depth / texture_width, (box_height + box_depth) / texture_height},
        {(box_depth + box_width) / texture_width,
         (2 * box_depth + box_height) / texture_height},
        {(box_depth + box_width) / texture_width,
         (box_height + box_depth) / texture_height},
        // Top
        {box_depth / texture_width, 0 / texture_height},
        {box_depth / texture_width, box_depth / texture_height},
        {(box_depth + box_width) / texture_width, 0 / texture_height},
        {(box_depth + box_width) / texture_width, box_depth / texture_height},
        // Back
        {2 * (box_depth + box_width) / texture_width,
         (box_depth + box_height) / texture_height},
        {2 * (box_depth + box_width) / texture_width,
         box_depth / texture_height},
        {(2 * box_depth + box_width) / texture_width,
         (box_depth + box_height) / texture_height},
        {(2 * box_depth + box_width) / texture_width,
         box_depth / texture_height},
        // Front
        {box_depth / texture_width,
         (box_depth + box_height) / texture_height},              // 1
        {box_depth / texture_width, box_depth / texture_height},  // 3
        {(box_depth + box_width) / texture_width,
         (box_depth + box_height) / texture_height},  // 2
        {(box_depth + box_width) / texture_width,
         box_depth / texture_height},  // 4

    };

    for (int i = 0; i < 6; i++) {
        float choice = (i % 2 ? 1 : -1);
        int pos = i / 2;
        int count = 0;
        for (int j = 0; j < 8; j++) {
            if (positions[j][pos] == choice) {
                vDef.push_back({positions[j] * scale_factor, normals[i],
                                UVs[i * 4 + count]});
                count++;
            }
        }

        vIdx.push_back(i * 4);
        vIdx.push_back(i * 4 + 1);
        vIdx.push_back(i * 4 + 2);
        vIdx.push_back(i * 4 + 1);
        vIdx.push_back(i * 4 + 2);
        vIdx.push_back(i * 4 + 3);
    }
}

// #define M_PI 3.141595f
void Assignment15::createSphereMesh(std::vector<Vertex> &vDef,
                                    std::vector<uint32_t> &vIdx) {
    // The primitive built here is a sphere of radius 1, centered in the origin,
    // on which the Mars texture is applied seamless.

    // The procedure fills array vPos with the positions of the vertices and of
    // the normal vectors of the mesh
    for (int i = 0; i <= 180; i++) {
        for (int j = 0; j <= 360; j++) {
            float a = i * M_PI / 180.0f;
            float b = j * M_PI / 180.0f;

            float x = sin(a) * cos(b);
            float z = sin(a) * sin(b);
            float y = cos(a);
            glm::vec3 surface = glm::vec3({x, y, z});
            glm::vec3 normal = glm::normalize(surface);
            vDef.push_back({surface, normal, {j / 360.0f, i / 180.0f}});
        }
    }

    // The procedures also fill the array vIdx with the indices of the vertices
    // of the triangles

    for (int i = 0; i < 180; i++) {
        for (int j = 0; j < 360; j++) {
            vIdx.push_back(i * 361 + j);
            vIdx.push_back(i * 361 + j + 1);
            vIdx.push_back((i + 1) * 361 + j);
            vIdx.push_back(i * 361 + j + 1);
            vIdx.push_back((i + 1) * 361 + j);
            vIdx.push_back((i + 1) * 361 + j + 1);
        }
    }
}
