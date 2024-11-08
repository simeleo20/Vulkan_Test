include .env

CFLAGS = -std=c++17 -O2 -I "E:/Visual Studio/Visual Studio/Libraries/glfw-3.4.bin.WIN64/include" -I "c:/VulkanSDK/1.3.296.0/Include" -I "E:/Visual Studio/Visual Studio/Libraries/glm"
LDFLAGS = -L"E:/Visual Studio/Visual Studio/Libraries/glfw-3.4.bin.WIN64/lib-mingw-w64" -L $(VULKAN_SDK) -lglfw3 -lvulkan-1 -lgdi32

# create list of all spv files and set as dependency
vertSources = $(shell powershell -Command "(Get-ChildItem -Path .\shaders -Filter '*.vert' -Recurse | Select-Object -ExpandProperty FullName ).replace('\','/')")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell powershell -Command "(Get-ChildItem -Path .\shaders -Filter '*.frag' -Recurse | Select-Object -ExpandProperty FullName ).replace('\','/')")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

$(info $$DATA = $(fragObjFiles))

TARGET = out.exe
$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): *.cpp *.hpp
	g++ $(CFLAGS) -o $(TARGET) *.cpp $(LDFLAGS)

# make shader targets
%.spv: %
	${GLSLC} $< -o $@

.PHONY: test clean

test: out.exe
	./out.exe

clean:
	rm -f out.exe
	rm -f *.spv


