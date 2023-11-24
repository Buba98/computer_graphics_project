
void Assignment15::createBoxMesh(std::vector <Vertex> &vDef, std::vector <uint32_t> &vIdx) {
    // The primitive built here is a box centered in the origin, with proportions respecting the texture.

    // The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
    float texture_width = 1024;
    float texture_height = 512;

    float box_width = 350;
    float box_height = 324;
    float box_depth = 94;

    float scale_factor = box_depth;

    float scaled_box_width = box_width / scale_factor;
    float scaled_box_height = box_height / scale_factor;
    float scaled_box_depth = box_depth / scale_factor;

    //Front

    vDef.push_back({{-scaled_box_width / 2,     -scaled_box_height / 2, scaled_box_depth / 2},
                    {0.0f,                      0.0f,                   1.0f},
                    {box_depth / texture_width, (box_depth + box_height) / texture_height}});
    vDef.push_back({{scaled_box_width / 2,                    -scaled_box_height / 2, scaled_box_depth / 2},
                    {0.0f,                                    0.0f,                   1.0f},
                    {(box_depth + box_width) / texture_width, (box_depth + box_height) / texture_height}});
    vDef.push_back({{-scaled_box_width / 2,     scaled_box_height / 2, scaled_box_depth / 2},
                    {0.0f,                      0.0f,                  1.0f},
                    {box_depth / texture_width, box_depth / texture_height}});
    vDef.push_back({{scaled_box_width / 2,                    scaled_box_height / 2, scaled_box_depth / 2},
                    {0.0f,                                    0.0f,                  1.0f},
                    {(box_depth + box_width) / texture_width, box_depth / texture_height}});

    vIdx.push_back(0);
    vIdx.push_back(1);
    vIdx.push_back(2);
    vIdx.push_back(1);
    vIdx.push_back(3);
    vIdx.push_back(2);

    //Back

    vDef.push_back({{-scaled_box_width / 2,                       -scaled_box_height / 2, -scaled_box_depth / 2},
                    {0.0f,                                        0.0f,                   -1.0f},
                    {2 * (box_depth + box_width) / texture_width, (box_depth + box_height) / texture_height}});
    vDef.push_back({{scaled_box_width / 2,                        -scaled_box_height / 2, -scaled_box_depth / 2},
                    {0.0f,                                        0.0f,                   -1.0f},
                    {(2 * box_depth + box_width) / texture_width, (box_depth + box_height) / texture_height}});
    vDef.push_back({{-scaled_box_width / 2,                       scaled_box_height / 2, -scaled_box_depth / 2},
                    {0.0f,                                        0.0f,                  -1.0f},
                    {2 * (box_depth + box_width) / texture_width, box_depth / texture_height}});
    vDef.push_back({{scaled_box_width / 2,                        scaled_box_height / 2, -scaled_box_depth / 2},
                    {0.0f,                                        0.0f,                  -1.0f},
                    {(2 * box_depth + box_width) / texture_width, box_depth / texture_height}});

    vIdx.push_back(4);
    vIdx.push_back(5);
    vIdx.push_back(6);
    vIdx.push_back(5);
    vIdx.push_back(7);
    vIdx.push_back(6);

    //Left

    vDef.push_back({{-scaled_box_width / 2, -scaled_box_height / 2, -scaled_box_depth / 2},
                    {-1.0f,                 0.0f,                   0.0f},
                    {0.0f,                  (box_depth + box_height) / texture_height}});
    vDef.push_back({{-scaled_box_width / 2,     -scaled_box_height / 2, scaled_box_depth / 2},
                    {-1.0f,                     0.0f,                   0.0f},
                    {box_depth / texture_width, (box_depth + box_height) / texture_height}});
    vDef.push_back({{-scaled_box_width / 2, scaled_box_height / 2, -scaled_box_depth / 2},
                    {-1.0f,                 0.0f,                  0.0f},
                    {0.0f,                  box_depth / texture_height}});
    vDef.push_back({{-scaled_box_width / 2,     scaled_box_height / 2, scaled_box_depth / 2},
                    {-1.0f,                     0.0f,                  0.0f},
                    {box_depth / texture_width, box_depth / texture_height}});

    vIdx.push_back(8);
    vIdx.push_back(9);
    vIdx.push_back(10);
    vIdx.push_back(9);
    vIdx.push_back(11);
    vIdx.push_back(10);

    //Right

    vDef.push_back({{scaled_box_width / 2,                        -scaled_box_height / 2, -scaled_box_depth / 2},
                    {1.0f,                                        0.0f,                   0.0f},
                    {(2 * box_depth + box_width) / texture_width, (box_depth + box_height) / texture_height}});
    vDef.push_back({{scaled_box_width / 2,                    -scaled_box_height / 2, scaled_box_depth / 2},
                    {1.0f,                                    0.0f,                   0.0f},
                    {(box_depth + box_width) / texture_width, (box_depth + box_height) / texture_height}});
    vDef.push_back({{scaled_box_width / 2,                        scaled_box_height / 2, -scaled_box_depth / 2},
                    {1.0f,                                        0.0f,                  0.0f},
                    {(2 * box_depth + box_width) / texture_width, box_depth / texture_height}});
    vDef.push_back({{scaled_box_width / 2,                    scaled_box_height / 2, scaled_box_depth / 2},
                    {1.0f,                                    0.0f,                  0.0f},
                    {(box_depth + box_width) / texture_width, box_depth / texture_height}});

    vIdx.push_back(12);
    vIdx.push_back(13);
    vIdx.push_back(14);
    vIdx.push_back(13);
    vIdx.push_back(15);
    vIdx.push_back(14);

    //Top

    vDef.push_back({{-scaled_box_width / 2,     scaled_box_height / 2, -scaled_box_depth / 2},
                    {0.0f,                      1.0f,                  0.0f},
                    {box_depth / texture_width, 0 / texture_height}});
    vDef.push_back({{scaled_box_width / 2,                    scaled_box_height / 2, -scaled_box_depth / 2},
                    {0.0f,                                    1.0f,                  0.0f},
                    {(box_depth + box_width) / texture_width, 0 / texture_height}});
    vDef.push_back({{-scaled_box_width / 2,     scaled_box_height / 2, scaled_box_depth / 2},
                    {0.0f,                      1.0f,                  0.0f},
                    {box_depth / texture_width, box_depth / texture_height}});
    vDef.push_back({{scaled_box_width / 2,                    scaled_box_height / 2, scaled_box_depth / 2},
                    {0.0f,                                    1.0f,                  0.0f},
                    {(box_depth + box_width) / texture_width, box_depth / texture_height}});

    vIdx.push_back(16);
    vIdx.push_back(17);
    vIdx.push_back(18);
    vIdx.push_back(17);
    vIdx.push_back(19);
    vIdx.push_back(18);

    //Bottom

    vDef.push_back({{-scaled_box_width / 2,     -scaled_box_height / 2, -scaled_box_depth / 2},
                    {0.0f,                      -1.0f,                  0.0f},
                    {box_depth / texture_width, (2 * box_depth + box_height) / texture_height}});
    vDef.push_back({{scaled_box_width / 2,                    -scaled_box_height / 2, -scaled_box_depth / 2},
                    {0.0f,                                    -1.0f,                  0.0f},
                    {(box_depth + box_width) / texture_width, (2 * box_depth + box_height) / texture_height}});
    vDef.push_back({{-scaled_box_width / 2,     -scaled_box_height / 2, scaled_box_depth / 2},
                    {0.0f,                      -1.0f,                  0.0f},
                    {box_depth / texture_width, (box_height + box_depth) / texture_height}});
    vDef.push_back({{scaled_box_width / 2,                    -scaled_box_height / 2, scaled_box_depth / 2},
                    {0.0f,                                    -1.0f,                  0.0f},
                    {(box_depth + box_width) / texture_width, (box_height + box_depth) / texture_height}});

    vIdx.push_back(20);
    vIdx.push_back(21);
    vIdx.push_back(22);
    vIdx.push_back(21);
    vIdx.push_back(23);
    vIdx.push_back(22);
}

//#define M_PI 3.141595f
void Assignment15::createSphereMesh(std::vector <Vertex> &vDef, std::vector <uint32_t> &vIdx) {
    // The primitive built here is a sphere of radius 1, centered in the origin, on which the Mars texture is applied seamless.

    // The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
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


    // The procedures also fill the array vIdx with the indices of the vertices of the triangles

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

