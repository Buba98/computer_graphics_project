CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
SOURCE = $(word 2,$(MAKECMDGOALS))
HEADERS = headers/

compile: $(SOURCE)/$(SOURCE).cpp 
	cd $(SOURCE) && g++ -I $(HEADERS) $(CFLAGS) -o VulkanProgram $(SOURCE).cpp $(LDFLAGS)

.PHONY: test clean

run: $(SOURCE)/VulkanProgram
	cd $(SOURCE) && ./VulkanProgram

clean:
	cd $(SOURCE) && rm -f VulkanProgram

all: clean compile run