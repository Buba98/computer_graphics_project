#!/bin/bash
if [ "$1" == 2 ]; then
  # shellcheck disable=SC2164
  cd shaders
  glslc PhongShader.v2.frag -o PhongFrag.spv
  glslc PhongShader.v2.vert -o PhongVert.spv
  # shellcheck disable=SC2103
  cd ..
else
  # shellcheck disable=SC2164
  cd shaders
  glslc BlinnShader1.frag -o BlinnFrag1.spv
  glslc BlinnShader2.frag -o BlinnFrag2.spv
  glslc BlinnShader3.frag -o BlinnFrag3.spv
  # shellcheck disable=SC2103
  cd ..
fi
