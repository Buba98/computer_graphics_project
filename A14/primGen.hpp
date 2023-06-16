void Assignment14::createCubeMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The primitive built here is a cube centered in the origin, whose edge length is 2.

	// Fill array vPos with the positions of the vertices of the mesh
	vDef.push_back({{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 0 - Position and Normal
	vDef.push_back({{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 1 - Position and Normal
	vDef.push_back({{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 2 - Position and Normal
	vDef.push_back({{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});  // vertex 3 - Position and Normal

	// Fill the array vIdx with the indices of the vertices of the triangles
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle

    // Fill array vPos with the positions of the vertices of the mesh
    vDef.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});	// vertex 4 - Position and Normal
    vDef.push_back({{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});	// vertex 5 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});	// vertex 6 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});  // vertex 7 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(4); vIdx.push_back(5); vIdx.push_back(6);	// First triangle
    vIdx.push_back(5); vIdx.push_back(6); vIdx.push_back(7);	// Second triangle

    // Fill array vPos with the positions of the vertices of the mesh
    vDef.push_back({{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}});	// vertex 8 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}});	// vertex 9 - Position and Normal
    vDef.push_back({{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});	// vertex 10 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});  // vertex 11 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(8); vIdx.push_back(9); vIdx.push_back(10);	// First triangle
    vIdx.push_back(9); vIdx.push_back(10); vIdx.push_back(11);	// Second triangle

    // Fill array vPos with the positions of the vertices of the mesh
    vDef.push_back({{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}});	// vertex 12 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}});	// vertex 13 - Position and Normal
    vDef.push_back({{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}});	// vertex 14 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}});  // vertex 15 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(12); vIdx.push_back(13); vIdx.push_back(14);	// First triangle
    vIdx.push_back(13); vIdx.push_back(14); vIdx.push_back(15);	// Second triangle

    // Fill array vPos with the positions of the vertices of the mesh
    vDef.push_back({{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});	// vertex 16 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});	// vertex 17 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});	// vertex 18 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});  // vertex 19 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(16); vIdx.push_back(17); vIdx.push_back(18);	// First triangle
    vIdx.push_back(17); vIdx.push_back(18); vIdx.push_back(19);	// Second triangle

    // Fill array vPos with the positions of the vertices of the mesh
    vDef.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}});	// vertex 20 - Position and Normal
    vDef.push_back({{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}});	// vertex 21 - Position and Normal
    vDef.push_back({{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}});	// vertex 22 - Position and Normal
    vDef.push_back({{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}});  // vertex 23 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(20); vIdx.push_back(21); vIdx.push_back(22);	// First triangle
    vIdx.push_back(21); vIdx.push_back(22); vIdx.push_back(23);	// Second triangle

}

void Assignment14::createFunctionMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// The primitive built here is the surface y = sin(x) * cos(z) with -3 <= x <= 3 and -3 <= z <= 3.

	// Fill array vPos with the positions of the vertices of the mesh
	vDef.push_back({{0.0f, 0.0f, 0.1f}, {0.5f, 0.0f, 0.866f}});	// vertex 0 - Position and Normal
	vDef.push_back({{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 1 - Position and Normal
	vDef.push_back({{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 2 - Position and Normal

	// Fill the array vIdx with the indices of the vertices of the triangles
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
}

void Assignment14::createCylinderMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// The primitive built here is a cylinder, with radius 1, and height 2, centered in the origin.

	// Fill array vPos with the positions of the vertices of the mesh
	vDef.push_back({{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 0 - Position and Normal
	vDef.push_back({{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 1 - Position and Normal
	vDef.push_back({{1.0f, 1.0f, 0.1f}, {-0.5f, 0.0f, 0.866f}});// vertex 2 - Position and Normal

	// Fill the array vIdx with the indices of the vertices of the triangles
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
}
