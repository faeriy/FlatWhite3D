#pragma once

#ifdef FW_PLATFORM_WINDOWS
	#ifdef FW_BUILD_DLL
		#define FLATWHITE3D_API __declspec(dllexport)
	#else
		#define FLATWHITE3D_API __declspec(dllimport)
	#endif
#else
	#error FlatWhite3D only supports Windows!
#endif