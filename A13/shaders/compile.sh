#!/bin/bash
# shellcheck disable=SC2164
#ls
#cd shaders
rm BlinnFrag.spv
glslc BlinnShader.frag -o BlinnFrag.spv
# shellcheck disable=SC2103
cd ..
