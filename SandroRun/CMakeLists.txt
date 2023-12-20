# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(SandroRun)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set the path to header files
include_directories(headers)

# Find required packages
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)

# Create a list of header files
file(GLOB HEADER_FILES headers/*.hpp headers/*.h)

# Create a list of source files
file(GLOB SOURCE_FILES src/*.cpp src/*.c)

# Add the main source file in the root folder
list(APPEND SOURCE_FILES main.cpp)

# Create the executable
set(TARGET_BINARY_NAME sandro_run)
add_executable(${TARGET_BINARY_NAME} ${HEADER_FILES}
        Starter.hpp BuildModels.hpp HandleCommands.hpp ${SOURCE_FILES}
        Car.hpp)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set_target_properties(${TARGET_BINARY_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Create directory for shaders
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Compile fragment shaders
file(GLOB SHADER_FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/*.frag)
foreach (FILE ${SHADER_FILES})
    string(REGEX REPLACE ".*/(.*)" "\\1" FILE_NAME ${FILE})
    string(REGEX REPLACE ".frag" "" FILE_NAME ${FILE_NAME})
    execute_process(
            COMMAND glslc ${FILE} -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${FILE_NAME}Frag.spv
    )
endforeach ()

# Compile vertex shaders
file(GLOB SHADER_FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/*.vert)
foreach (FILE ${SHADER_FILES})
    string(REGEX REPLACE ".*/(.*)" "\\1" FILE_NAME ${FILE})
    string(REGEX REPLACE ".vert" "" FILE_NAME ${FILE_NAME})
    execute_process(
            COMMAND glslc ${FILE} -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${FILE_NAME}Vert.spv
    )
endforeach ()

# Copy models and textures to the binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Link libraries
target_link_libraries(${TARGET_BINARY_NAME} glfw Vulkan::Vulkan glm::glm dl pthread X11 Xxf86vm Xrandr Xi)