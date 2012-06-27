// SulViewQuad.h

#ifndef __SULVIEWQUAD_H__
#define __SULVIEWQUAD_H__

// NOTE: deprecatd.. use the original SulScreenAlignedQuad

#include "SulTypes.h"
#include "SulScreenAlignedQuad.h"
#include <osgViewer/Viewer>

class CSulViewQuad : public osg::Group
{
public:
	CSulViewQuad();

	CSulViewQuad( osgViewer::Viewer* viewer, osg::Texture2D* tex, sigma::int32 x=0, sigma::int32 y=0, sigma::uint32 w=200, sigma::uint32 h=150 );

	// creates a quad that covers the whole view
	CSulViewQuad( osgViewer::Viewer* viewer );

	CSulGeomQuad* getQuad();

private:
	osg::ref_ptr<CSulScreenAlignedQuad>	m_saq;
};

#endif // __SULVIEWQUAD_H__