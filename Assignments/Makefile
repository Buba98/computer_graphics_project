# Compiler flags
CFLAGS = -std=c++17 -O2
# Linker flags
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
# Source directory
SOURCE = $(word 2,$(MAKECMDGOALS))
# Headers directory
HEADERS = headers/

# Target for compilation
compile: $(SOURCE)/$(SOURCE).cpp
	cd $(SOURCE) && g++ -I $(HEADERS) $(CFLAGS) -o VulkanProgram $(SOURCE).cpp $(LDFLAGS)

# Target for running the program
run: $(SOURCE)/VulkanProgram
	cd $(SOURCE) && ./VulkanProgram

# Target for cleaning up
clean:
	rm -f $(SOURCE)/VulkanProgram
	cd $(SOURCE)/shaders && ./clean_shaders.sh

# Target for compiling shaders
shaders:
	cd $(SOURCE)/shaders && ./compile.sh

# Phony target for executing all targets
.PHONY: all clean shaders compile run
all: clean shaders compile run
