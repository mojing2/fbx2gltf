# fbx2gltf

command tool used to convert FBX to glTF

# build

	vcpkg install zlib:x64-windows cpprestsdk:x64-windows
	mkdir build
	cd build
	cmake ../src -G "Visual Studio 15 2017 Win64" -DCMAKE_TOOLCHAIN_FILE=D:\workspace\vcpkg\scripts\buildsystems\vcpkg.cmake