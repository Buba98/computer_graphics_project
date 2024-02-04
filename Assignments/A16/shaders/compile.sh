#!/bin/bash

glslc Mesh.frag -o MeshFrag.spv
glslc Mesh.vert -o MeshVert.spv

glslc Overlay.frag -o OverlayFrag.spv
glslc Overlay.vert -o OverlayVert.spv

glslc VColor.frag -o VColorFrag.spv
glslc VColor.vert -o VColorVert.spv