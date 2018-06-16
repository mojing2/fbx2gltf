//
// Copyright (c) Autodesk, Inc. All rights reserved 
//
// C++ glTF FBX converter
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
#include "getopt.h"
#if defined(_WIN32) || defined(_WIN64)
#include "tchar.h"
#endif

// Tests
// -f $(ProjectDir)\..\models\duck\duck.fbx -o $(ProjectDir)\..\models\duck\out -n test -c
// -f $(ProjectDir)\..\models\au\au3.fbx -o $(ProjectDir)\..\models\au\out -n test -c
// -f $(ProjectDir)\..\models\teapot\teapot.fbx -o $(ProjectDir)\..\models\teapot\out -n test -c
// -f $(ProjectDir)\..\models\wine\wine.fbx -o $(ProjectDir)\..\models\wine\out -n test -c
// -f $(ProjectDir)\..\models\monster\monster.fbx -o $(ProjectDir)\..\models\monster\out -n test -c
// -f $(ProjectDir)\..\models\Carnivorous_plant\Carnivorous_plant.fbx -o $(ProjectDir)\..\models\Carnivorous_plant\out -n test -c
//  -f C:\Users\XXXXXX\Desktop\xxxxxx\Skeleton\skeletonModel\0825a.FBX -e -a

	//{ U("n"), U("a"), required_argument, U("-a -> export animations, argument [bool], default:true") },
	//{ U("n"), U("g"), required_argument, U("-g -> [experimental] GLSL version to output in generated shaders") },
	//{ U("n"), U("d"), no_argument, U("-d -> export pass details to be able to regenerate shaders and states") },
	//{ U("n"), U("c"), required_argument, U("-c -> compression type: available: Open3DGC [string]") },
	//{ U("n"), U("m"), required_argument, U("-m -> compression mode: for Open3DGC can be \"ascii\"(default) or \"binary\" [string]") },
	//{ U("n"), U("n"), no_argument, U("-n -> don't combine animations with the same target") }

void usage () {
	ucout << std::endl << U("glTF [-h] [-v] [-n] [-d] [-t] [-l] [-c] [-e] [-a] [-w] [-o <output path>] -f <input file>") << std::endl ;
	ucout << U("-f/--file \t\t- file to convert to glTF [string]") << std::endl ;
	ucout << U("-o/--output \t\t- path of output directory [string]") << std::endl ;
	ucout << U("-n/--name \t\t- override the scene name [string]") << std::endl ;
	ucout << U("-d/--degree \t\t- output angles in degrees vs radians (default to radians)") << std::endl ;
	ucout << U("-t/--transparency \t- invert transparency") << std::endl ;
	//ucout << U("-l/--lighting \t- enable default lighting (if no lights in scene)") << std::endl ;
	ucout << U("-c/--copy \t\t- copy all media to the target directory (cannot be combined with --embed)") << std::endl ;
	ucout << U("-e/--embed \t\t- embed all resources as Data URIs (cannot be combined with --copy)") << std::endl ;
	ucout << U("-h/--help \t\t- this message") << std::endl ;
	ucout << U("-v/--version \t\t- version") << std::endl ;
	ucout << U("-a/--android \t\t- android platform") << std::endl;
	ucout << U("-w/--wondows \t\t- windows platform") << std::endl;
	ucout << U("-i/--ios \t\t- ios platform") << std::endl;
	ucout << U("-g/--windows nothing \t\t- only windows nothing compress format") << std::endl;
}
static struct option long_options [] ={
	{ U("file"), ARG_REQ, 0, U('f') },
	{ U("output"), ARG_REQ, 0, U('o') },
	{ U("name"), ARG_REQ, 0, U('n') },
	{ U("degree"), ARG_NONE, 0, U('d') },
	{ U("transparency"), ARG_NONE, 0, U('t') },
	{ U("lighting"), ARG_NONE, 0, U('l') },
	{ U("copy"), ARG_NONE, 0, U('c') },
	{ U("embed"), ARG_NONE, 0, U('e') },
	{ U("help"), ARG_NONE, 0, U('h') },
	{ U("version"), ARG_NONE, 0, U('v') },
	{ U("android_platform"), ARG_NONE, 0, U('a') },
	{ U("windows_platform"), ARG_NONE, 0, U('w') },
	{ U("ios_platform"), ARG_NONE, 0, U('i') },
	{ U("nothing"), ARG_NONE, 0, U('g') },

	{ ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL }
} ;

#if defined(_WIN32) || defined(_WIN64)
int _tmain (int argc, _TCHAR *argv []) {
#else
int main (int argc, char *argv []) {
#endif
	bool bLoop =true ;
	utility::string_t inFile ;
	utility::string_t outDir ;
	utility::string_t name ;
	bool angleInDegree =false ;
	bool reverseTransparency =false ;
	bool defaultLighting =false ;
	bool copyMedia =false ;
	bool embedMedia =false ;

	FILE *fp;
	fp = fopen("config.log", "w");

	while ( bLoop ) {
		int option_index =0 ;
		// http://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html
		// http://www.gnu.org/software/libc/manual/html_node/Argp-Examples.html#Argp-Examples
		// http://stackoverflow.com/questions/13251732/c-how-to-specify-an-optstring-in-the-getopt-function
		int c =getopt_long (argc, argv, U ("f:o:n:tlcehvawig"), long_options, &option_index) ;
		//printf("%c", c);
		// Check for end of operation or error
		if ( c == -1 )
			break ;
		
		// Handle options
		switch ( c ) {
			case 0:
				break ;
			case U('?'):
				// getopt_long already printed an error message.
				break ;
			case U(':'): // missing option argument
				ucout << U("option \'") << optopt << U("\' requires an argument") << std::endl ;
				break ;
			default:
				bLoop =false ;
				break ;
				
			case U('h'): // help message
				usage () ;
				return (0) ;
			case U('f'): // file to convert to glTF [string]
				inFile =optarg ;
				break ;
			case U('o'): // path of output directory argument [string]
				outDir =optarg ;
				break ;
			case U('n'): // override the scene name [string]
				name =optarg ;
				break ;
			case U('d'): // invert transparency
				angleInDegree =true ;
				break ;
			case U('t'): // invert transparency
				reverseTransparency =true ;
				break ;
			case U('l'): // enable default lighting (if no lights in scene)
				defaultLighting =true ;
				break ;
			case U('c'): // copy all media to the target directory (cannot be combined with --embed)
				copyMedia =!embedMedia ;
				break ;
			case U('e'): // embed all resources as Data URIs (cannot be combined with --copy)
				embedMedia =!copyMedia ;
				break ;
			case U('a'): // android platform
				//printf("android platform\n");
				fwrite("android_platform", strlen("android_platform"), 1, fp);
				break;
			case U('w'): // windows platform
				//printf("pc platform\n");
				fwrite("windows_platform", strlen("windows_platform"), 1, fp);
				break;
			case U('g'): // windows nothing compress format
				fwrite("_nothing", strlen("_nothing"), 1, fp);
				break;
			case U('i'): // windows platform
				//printf("ios platform\n");
				fwrite("ios_platform", strlen("ios_platform"), 1, fp);
				break;
		}
	}
	fclose(fp);

#if defined(_WIN32) || defined(_WIN64)
	if ( inFile.length () == 0  || _taccess_s (inFile.c_str (), 0) == ENOENT )
		return (-1) ;
#else
	if ( inFile.length () == 0  || access (inFile.c_str (), 0) == ENOENT )
		return (-1) ;
#endif
	if ( outDir.length () == 0 )
		outDir =utility::conversions::to_string_t (FbxPathUtils::GetFolderName (utility::conversions::to_utf8string (inFile).c_str ()).Buffer ()) ;
#if defined(_WIN32) || defined(_WIN64)
	if ( outDir [outDir.length () - 1] != U('\\') )
		outDir += U('\\') ;
#else
	if ( outDir [outDir.length () - 1] != U('/') )
		outDir +=U('/') ;
#endif
	
	std::shared_ptr <gltfPackage> asset (new gltfPackage ()) ;
	asset->ioSettings (name.c_str (), angleInDegree, reverseTransparency, defaultLighting, copyMedia, embedMedia) ;

	ucout << U("Loading file: ") << inFile << U("...") << std::endl ;
	bool bRet =asset->load (inFile) ;
	ucout << U("Converting to GLTF ...") << std::endl ;
	bRet =asset->save (outDir) ;
	ucout << U("done!") << std::endl ;

	return (0) ;
}

