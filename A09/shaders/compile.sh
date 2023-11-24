#!/bin/bash

rm -f PhongFrag.spv
rm -f PhongVert.spv

if [ "$1" = "2" ]; then
  glslc PhongShader.frag -o PhongFrag.spv
  glslc PhongShader.vert -o PhongVert.spv
else
  glslc PhongShader.v2.frag -o PhongFrag.spv
  glslc PhongShader.v2.vert -o PhongVert.spv
fi
