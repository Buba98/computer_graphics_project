#!/bin/bash
# shellcheck disable=SC2164
cd shaders
glslc BlinnShader.frag -o BlinnFrag.spv
# shellcheck disable=SC2103
cd ..
