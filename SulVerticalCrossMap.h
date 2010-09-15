// SulVerticalCrossMap.h

#ifndef __SULVERTICALCROSSMAP_H__
#define __SULVERTICALCROSSMAP_H__

#include "SulString.h"
#include <osg/TextureCubeMap>

class CSulVerticalCrossMap : public osg::TextureCubeMap
{
public:
				CSulVerticalCrossMap( const CSulString& file );

	bool		loadVerticalCrossMap( const CSulString& file );

	osg::Image*	getImage( osg::TextureCubeMap::Face face );

private:
	osg::ref_ptr<osg::Image> zplus;
	osg::ref_ptr<osg::Image> zminus;
	osg::ref_ptr<osg::Image> yplus;
	osg::ref_ptr<osg::Image> yminus;
	osg::ref_ptr<osg::Image> xplus;
	osg::ref_ptr<osg::Image> xminus;
};


#endif // __SULVERTICALCROSSMAP_H__