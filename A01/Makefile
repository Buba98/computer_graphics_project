CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
HEADERS = headers/

compile: $(SOURCE) 
	g++ -I $(HEADERS) $(CFLAGS) -o VulkanProgram $(SOURCE) $(LDFLAGS)

.PHONY: test clean

run: VulkanProgram
	./VulkanProgram

clean: VulkanProgram
	rm -f VulkanProgram
