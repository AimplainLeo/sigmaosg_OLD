// SulViewTexture.h

#ifndef __SULVIEWTEXTURE_H__
#define __SULVIEWTEXTURE_H__

// DEPRECATED: use CSulViewQuad

#include "SulTypes.h"
#include "SulScreenAlignedQuad.h"
#include <osgViewer/Viewer>

class CSulViewTexture : public osg::Group
{
public:
	CSulViewTexture( osgViewer::Viewer* viewer, osg::Texture2D* tex, sigma::int32 x=0, sigma::int32 y=0, sigma::uint32 w=200, sigma::uint32 h=150 );

	// creates a quad that covers the whole view
	CSulViewTexture::CSulViewTexture( osgViewer::Viewer* viewer );

	CSulGeomQuad* getQuad();

private:
	osg::ref_ptr<CSulScreenAlignedQuad>	m_saq;
};

#endif // __SULVIEWTEXTURE_H__