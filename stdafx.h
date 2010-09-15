// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <memory.h>
#include <algorithm>

#include <osg/AlphaFunc>
#include <osg/animationpath>
#include <osg/Camera>
#include <osg/MatrixTransform>
#include <osg/Notify>
#include <osg/Texture>
#include <osg/Texture2D>
#include <osg/Projection>
#include <osg/ref_ptr>

#include <osgGA/TrackballManipulator>

// if win32 but not vs2003
#if defined(WIN32) && !(_MSC_VER>=1310 && _MSC_VER<1400)
	// this will allow us to use microsofts memory leak debugging code
	#ifdef _DEBUG
	#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
	#else
	#define DEBUG_CLIENTBLOCK
	#endif
#else
	#define DEBUG_CLIENTBLOCK new
#endif