CFLAGS = -std=c++17 -O2 -I "E:/Visual Studio/Visual Studio/Libraries/glfw-3.4.bin.WIN64/include" -I "c:/VulkanSDK/1.3.296.0/Include" -I "E:/Visual Studio/Visual Studio/Libraries/glm"
LDFLAGS = -L"E:/Visual Studio/Visual Studio/Libraries/glfw-3.4.bin.WIN64/lib-mingw-w64" -L"c:/VulkanSDK/1.3.296.0/Lib" -lglfw3 -lvulkan-1 -lgdi32

build: *.cpp *.hpp
	g++ $(CFLAGS) -o out *.cpp $(LDFLAGS)

.PHONY: test clean

test: build
	./out.exe

clean:
	del out.exe

	