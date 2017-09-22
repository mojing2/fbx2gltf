cmake_minimum_required( VERSION 3.8.2 )

project ( IO-glTF )

add_library( IO-glTF SHARED
	dllmain.cpp
	glslShader.cpp
	gltfReader.cpp
	gltfWriter-Animation.cpp
	gltfWriter-Asset.cpp
	gltfWriter-Buffer.cpp
	gltfWriter-Camera.cpp
	gltfWriter-Light.cpp
	gltfWriter-Line.cpp
	gltfWriter-Material.cpp
	gltfWriter-Mesh.cpp
	gltfWriter-Null.cpp
	gltfWriter-Program.cpp
	gltfWriter-Scene.cpp
	gltfWriter-Shaders.cpp
	gltfWriter-Skin.cpp
	gltfWriter-Technique.cpp
	gltfWriter-Texture.cpp
	gltfWriter.cpp
	gltfWriterVBO.cpp
	IOglTF.cpp
	JsonPrettify.cpp
	StdAfx.cpp
)

target_compile_definitions( IO-glTF PUBLIC
	_WIN64 
	_GLTF_EXPORTS_ 
	_WINDOWS
	_USRDLL
)
#target_compile_options( IO-glTF PRIVATE -Wall )

#
# FBX SDK
#
target_include_directories( IO-glTF PUBLIC $ENV{FBX_SDK}/include )
target_link_libraries( IO-glTF PUBLIC 
	debug $ENV{FBX_SDK}/lib/vs2015/x64/debug/libfbxsdk.lib
	optimized $ENV{FBX_SDK}/lib/vs2015/x64/release/libfbxsdk.lib
)

#
# CppREST SDK
#
find_path( CPPREST_INCLUDE cpprest/http_client.h )
find_library( CPPREST_LIB NAMES cpprest_2_9d cpprest_2_9 cpprestd cpprest )
target_include_directories( IO-glTF PUBLIC ${CPPREST_INCLUDE} )
target_link_libraries( IO-glTF PUBLIC ${CPPREST_LIB} )
