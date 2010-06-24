// SulTypes.h

#ifndef __SULTYPES_H__
#define __SULTYPES_H__

#include "SulDataTri.h"
#include <osg/linesegment>
#include <osg/plane>
#include <osg/texture2d>
#include <osg/stateattribute>
#include <vector>
#include <map>

namespace Sigma
{
	typedef char			int8;		// char:			-127 -> 128
	typedef short			int16;		// short:			-??? -> ???
	typedef int				int32;		// int:				-??? -> ???

	typedef unsigned char	uint8;		// unsgined char:	0 -> 255
	typedef unsigned short	uint16;		// unsigned short:	??
	typedef unsigned int	uint32;		// unsigned int:	0 -> 4294967295

	typedef std::vector< osg::Vec3 >									VEC_VEC3;
	typedef std::vector< CSulDataTri >									VEC_TRI;
	typedef std::vector< osg::ref_ptr<osg::LineSegment> >				VEC_LINESEGMENT;
	typedef std::vector< osg::Plane >									VEC_PLANE;
	typedef std::vector< osg::ref_ptr<osg::Texture2D> >					VEC_TEXTURE2D;
	typedef std::vector< osg::ref_ptr<osg::StateAttribute> >			VEC_STATEATTRIBUTE;
	typedef std::map< std::string, osg::ref_ptr<osg::StateAttribute> >	MAP_STRING_STATEATTRIBUTE;
	typedef std::map< uint32, osg::ref_ptr<osg::Texture2D> >			MAP_TEXTURE2D;

	#define RGBA( r, g, b, a ) (uint32)a<<24 | (uint32)b<<16 | (uint32)g<<8 | (uint32)r;

	#ifdef WIN32
		#define SSCANF		sscanf_s
		#define SPRINTF		sprintf_s
		#define STRICMP		_stricmp
		#define STRNICMP	_strnicmp
		#define STRCPY(a,b,c) 	strcpy_s(a,b,c)
		#define STRCAT(a,b,c) 	strcat_s(a,b,c)
		#define STRLWR	 	_strlwr_s
	// NOT vs2003
	#if !(_MSC_VER>=1310 && _MSC_VER<1400)
		#define VSPRINTF(a,b,c,d)	vsprintf_s(a,b,c,d)
	#else
		#define VSPRINTF(a,b,c,d) vsprintf(a,c,d)
	#endif

	#else 
		#define SSCANF		sscanf
		#define SPRINTF(a,b,c,d)	sprintf(a,c,d)
		#define STRICMP		strcasecmp
		#define STRCPY(a,b,c) 	strcpy(a,c)
		#define STRCAT(a,b,c) 	strcat(a,c)
		#define STRLWR(a,b)  std::string sss=a; std::transform(sss.begin(), sss.end(), sss.begin(), tolower );
		#define VSPRINTF(a,b,c,d) vsprintf(a,c,d)
	#endif 

}; // end namespace Sigma

#endif // __SULTYPES_H__
