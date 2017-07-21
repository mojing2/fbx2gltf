cmake_minimum_required( VERSION 3.8.0 )

project ( fbx2gltf )

#
# Reconfigure final output directory
#
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
message (STATUS "Ouput Directory: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_subdirectory( ./IO-glTF IO-glTF )

#
# Resource
#
set( RESOURCE_FILES
	glTF-1-0.json
	glTF-1-0-defaults.json
)
source_group( "Resource Files" FILES ${RESOURCE_FILES} )

#
# FBX SDK dll path
#
#if(NOT CMAKE_BUILD_TYPE)
#  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
#      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
#endif(NOT CMAKE_BUILD_TYPE)
#string ( TOLOWER ${CMAKE_BUILD_TYPE} glTF_BUILD_TYPE )
#set ( FBX_SDK_LIBS $ENV{FBX_SDK}/lib/vs2015/x64/${glTF_BUILD_TYPE} )
#link_directories ( ${FBX_SDK_LIBS} )

add_executable ( fbx2gltf
	getopt.cpp
	glTF.cpp
	gltfPackage.cpp
	StdAfx.cpp
	${RESOURCE_FILES}
)

target_compile_definitions( IO-glTF PUBLIC
	_UNICODE
	_WIN64
	_WINDOWS
	_USRDLL
)

target_compile_options( fbx2gltf PRIVATE -Wall )

target_link_libraries( fbx2gltf 
	IO-glTF
	debug $ENV{FBX_SDK}/lib/vs2015/x64/debug/libfbxsdk.lib
	optimized $ENV{FBX_SDK}/lib/vs2015/x64/release/libfbxsdk.lib
)

#
# Copy fbx dll to the same folder of .exe
#
add_custom_command( 
	TARGET fbx2gltf 
	POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$ENV{FBX_SDK}/lib/vs2015/x64/release/libfbxsdk.dll"
        $<TARGET_FILE_DIR:fbx2gltf>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/glTF-1-0-defaults.json
        $<TARGET_FILE_DIR:fbx2gltf>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/glTF-1-0.json
        $<TARGET_FILE_DIR:fbx2gltf>
)
