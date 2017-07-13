cmake_minimum_required( VERSION 3.8.2 )

project ( fbx2gltf )

add_executable ( fbx2gltf main.cpp )

target_compile_options( fbx2gltf PRIVATE -Wall )