//
// Copyright (c) Autodesk, Inc. All rights reserved 
//
// C++ glTF FBX importer/exporter plug-in
// by Cyrille Fauvel - Autodesk Developer Network (ADN)
// January 2015
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
#include "StdAfx.h"
#include "gltfWriter.h"
#include "glslShader.h"
#include <process.h>

namespace _IOglTF_NS_ {

	utility::string_t ConvertGlslToSpirv(utility::string_t& shader, utility::string_t& shaderName, glslShader::ShaderType stage)
	{
		wchar_t moduleFileName[1024];
		int bytes = GetModuleFileName(NULL, moduleFileName, 1024);
		assert(bytes != 0);
		utility::string_t currentDir(moduleFileName);

		// get shader file path
		utility::string_t glslName;
		if (stage == glslShader::ShaderType::Vertex)
		{
			glslName = shaderName + U(".vert");
		}
		else
		{
			glslName = shaderName + U(".frag");
		}

		utility::string_t inGlslPath = currentDir + U("\\..\\") + glslName;
		utility::string_t outSpirvPath = currentDir + U("\\..\\") + shaderName;

		// write shader to file
		FILE* shaderFile = NULL;
		_wfopen_s(&shaderFile, inGlslPath.c_str(), L"w");
		assert(shaderFile != NULL);
		fprintf_s(shaderFile, utility::conversions::to_utf8string(shader).c_str());
		fflush(shaderFile);
		fclose(shaderFile);

		utility::string_t PvrTexToolExe(U("D:\\app\\vulkan_sdk_1.0.54.0\\Bin\\glslc.exe"));
		utility::string_t cmd;
		cmd += PvrTexToolExe;
		cmd += U(" -std=310es ");
		cmd += U(" -o ");
		cmd += outSpirvPath;
		cmd += U(" ");
		cmd += inGlslPath;

		wchar_t scriptPath[1024];
		bytes = GetModuleFileName(NULL, scriptPath, 1024);
		assert(bytes != 0);
		wcscat_s(scriptPath, L"\\..\\glsl2spirv.bat");
		FILE * f = NULL;
		_wfopen_s(&f, scriptPath, L"w");
		if (f == NULL)
		{
			ucout << "open bat file failed!" << std::endl;
		}
		fprintf_s(f, utility::conversions::to_utf8string(cmd).c_str());
		fflush(f);
		fclose(f);

		auto result = _wspawnl(_P_WAIT, scriptPath, scriptPath, NULL);
		assert(result != -1);

		//image[name][U("uri")] = web::json::value::string(IOglTF::dataURI(outImagePath));

		return outSpirvPath;
	}

bool gltfWriter::WriteShaders () {
	//web::json::value buffer =web::json::value::object () ;
	//FbxString filename =FbxPathUtils::GetFileName (utility::conversions::to_utf8string (_fileName).c_str (), false) ;
	//buffer [U("name")] =web::json::value::string (utility::conversions::to_string_t (filename.Buffer ())) ;

	//buffer [U("uri")] =web::json::value::string (utility::conversions::to_string_t ((filename + ".bin").Buffer ())) ;
	//// The Buffer file should be fully completed by now.
	//if ( GetIOSettings ()->GetBoolProp (IOSN_FBX_GLTF_EMBEDMEDIA, false) ) {
	//	// data:[<mime type>][;charset=<charset>][;base64],<encoded data>
	//	buffer [U("uri")] =web::json::value::string (IOglTF::dataURI (_bin)) ;
	//}

	//if ( _writeDefaults )
	//	buffer [U("type")] =web::json::value::string (U("arraybuffer")) ; ; // default is arraybuffer
	//buffer [U("byteLength")] =web::json::value::number ((int)_bin.tellg ()) ;

	//_json [U("buffers")] [utility::conversions::to_string_t (filename.Buffer ())] =buffer ;

	//web::json::value techs =_json [U("techniques")] ;
	//for ( auto iter =techs.as_object ().begin () ; iter != techs.as_object ().end () ; iter++ ) {
	//	glslTech test (iter->second) ;

	//	ucout << test.vertexShader ().source ()
	//		<< std::endl ;
	//}


	web::json::value materials =_json [U("materials")] ;
	for ( auto iter =materials.as_object ().begin () ; iter != materials.as_object ().end () ; iter++ ) {
		utility::string_t name =iter->first ;
		utility::string_t techniqueName =iter->second [U("technique")].as_string () ;
		glslTech tech (
			_json [U("techniques")] [techniqueName],
			iter->second [U("values")],
			_json,
			U("310 es")
		) ;

		utility::string_t programName =_json [U("techniques")] [techniqueName] [U("program")].as_string () ;
		utility::string_t vsName =_json [U("programs")] [programName] [U("vertexShader")].as_string () ;
		utility::string_t fsName =_json [U("programs")] [programName] [U("fragmentShader")].as_string () ;
		
		FbxString gltfFilename (utility::conversions::to_utf8string (_fileName).c_str ()) ;
		
		if ( GetIOSettings ()->GetBoolProp (IOSN_FBX_GLTF_EMBEDMEDIA, false) ) {
			utility::string_t vertexShaderFilePath = ConvertGlslToSpirv(
				tech.vertexShader().source(), 
				vsName,
				glslShader::ShaderType::Vertex
			);
			utility::string_t fragmentShaderFilePath = ConvertGlslToSpirv(
				tech.fragmentShader().source(), 
				fsName,
				glslShader::ShaderType::Fragment
			);
			// data:[<mime type>][;charset=<charset>][;base64],<encoded data>
			_json [U("shaders")] [vsName] [U("uri")] =web::json::value::string (IOglTF::dataURI ( vertexShaderFilePath )) ;
			_json [U("shaders")] [fsName] [U("uri")] =web::json::value::string (IOglTF::dataURI ( fragmentShaderFilePath )) ;
		} else {
			utility::string_t vsFilename =_json [U("shaders")] [vsName] [U("uri")].as_string () ;
			{
				FbxString shaderFilename (utility::conversions::to_utf8string (vsFilename).c_str ()) ;
#if defined(_WIN32) || defined(_WIN64)
				shaderFilename =FbxPathUtils::GetFolderName (gltfFilename) + "\\" + shaderFilename ;
#else
				shaderFilename =FbxPathUtils::GetFolderName (gltfFilename) + "/" + shaderFilename ;
#endif
				std::wfstream shaderFile (shaderFilename, std::ios::out | std::ofstream::binary) ;
				//_bin.seekg (0, std::ios_base::beg) ;
#if defined(_WIN32) || defined(_WIN64)
				shaderFile.write (tech.vertexShader ().source ().c_str (), tech.vertexShader ().source ().length ()) ;
#else
				shaderFile << tech.vertexShader ().source ().c_str();

#endif
				shaderFile.close () ;
			}
			utility::string_t fsFileame =_json [U("shaders")] [fsName] [U("uri")].as_string () ;
			{
				FbxString shaderFilename (utility::conversions::to_utf8string (fsFileame).c_str ()) ;
#if defined(_WIN32) || defined(_WIN64)
				shaderFilename =FbxPathUtils::GetFolderName (gltfFilename) + "\\" + shaderFilename ;
#else
				shaderFilename =FbxPathUtils::GetFolderName (gltfFilename) + "/" + shaderFilename ;
#endif
				std::wfstream shaderFile (shaderFilename, std::ios::out | std::ofstream::binary) ;
				//_bin.seekg (0, std::ios_base::beg) ;
#if defined(_WIN32) || defined(_WIN64)
				shaderFile.write (tech.fragmentShader ().source ().c_str (), tech.fragmentShader ().source ().length ()) ;
#else
				shaderFile << tech.fragmentShader ().source ().c_str ();
#endif
				shaderFile.close () ;
			}
		}
	}
	return (true) ;
}

}
