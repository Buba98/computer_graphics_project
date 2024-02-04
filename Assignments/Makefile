CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
SOURCE = $(word 2,$(MAKECMDGOALS))
SOURCE2 = $(word 3,$(MAKECMDGOALS))
HEADERS = headers/

compile: $(SOURCE)/$(SOURCE).cpp
	cd $(SOURCE) && g++ -I $(HEADERS) $(CFLAGS) -o VulkanProgram $(SOURCE).cpp $(LDFLAGS)

.PHONY: test clean

run: $(SOURCE)/VulkanProgram
	cd $(SOURCE) && ./VulkanProgram

clean:
	cd $(SOURCE) && rm -f VulkanProgram

shaders: $(SOURCE)/shaders/compile.sh
	cd $(SOURCE)/shaders && ./compile.sh && cd ../..

all: clean compile run

all_shaders: clean shaders compile run