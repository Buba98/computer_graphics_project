#!/bin/bash
# shellcheck disable=SC2164
glslc PhongShader.frag -o PhongFrag.spv
glslc OrenNayarShader.frag -o OrenNayarFrag.spv
glslc BlinnNormMapShader.frag -o BlinnNormMapFrag.spv
glslc GGXNormMapShader.frag -o GGXNormMapFrag.spv
# shellcheck disable=SC2103
