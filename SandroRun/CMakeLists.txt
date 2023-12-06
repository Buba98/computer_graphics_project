# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(SandroRun)

# Set C++ standard and optimization level
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# Include directories
include_directories(headers)

# Find required packages
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)

# Set target binary name
set(TARGET_BINARY_NAME sandro_run)

# Add executable
add_executable(${TARGET_BINARY_NAME} SandroRun.cpp)

# Set runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set_target_properties(${TARGET_BINARY_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Create directory for shaders
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)

# Compile fragment shaders
file(GLOB SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag)
foreach (FILE ${SHADER_FILES})
    string(REGEX REPLACE ".*/(.*)" "\\1" FILE_NAME ${FILE})
    string(REGEX REPLACE ".frag" "" FILE_NAME ${FILE_NAME})
    execute_process(
            COMMAND glslc ${FILE} -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${FILE_NAME}Frag.spv
    )
endforeach ()

# Compile vertex shaders
file(GLOB SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert)
foreach (FILE ${SHADER_FILES})
    string(REGEX REPLACE ".*/(.*)" "\\1" FILE_NAME ${FILE})
    string(REGEX REPLACE ".vert" "" FILE_NAME ${FILE_NAME})
    execute_process(
            COMMAND glslc ${FILE} -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${FILE_NAME}Vert.spv
    )
endforeach ()

# Copy models and textures to binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Link libraries
target_link_libraries(${TARGET_BINARY_NAME} glfw Vulkan::Vulkan glm::glm dl pthread X11 Xxf86vm Xrandr Xi)
