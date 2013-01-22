// SulScreenAlignedQuad.h

#ifndef __SULSCREENALIGNEDQUAD_H__
#define __SULSCREENALIGNEDQUAD_H__

#include "SulExport.h"
#include "SulTypes.h"
#include "SulGeomQuad.h"
#include <osg/Referenced>
#include <osg/Group>
#include <osgViewer/Viewer>

class SUL_EXPORT CSulScreenAlignedQuad : public osg::Projection
{
public:
										CSulScreenAlignedQuad();
										
										CSulScreenAlignedQuad( float fViewW, float fViewH, osg::Texture2D* tex, sigma::int32 x=0, sigma::int32 y=0, sigma::uint32 w=200, sigma::uint32 h=150 );
										CSulScreenAlignedQuad( float fViewW, float fViewH, osg::Texture2D* tex, const osg::Vec3& pos, sigma::uint32 w=200, sigma::uint32 h=150 );

										CSulScreenAlignedQuad( osgViewer::Viewer* viewer, osg::Texture2D* tex, sigma::int32 x=0, sigma::int32 y=0, sigma::uint32 w=200, sigma::uint32 h=150 );

										CSulScreenAlignedQuad( const osg::Vec3& vPos, float w, float h, float fViewW, float fViewH );

										CSulScreenAlignedQuad( osgViewer::Viewer* viewer );

	osg::Group*							getGroup();
	CSulGeode*							getGeode();
	CSulGeomQuad*						getGeom();

	void								setTexture( const std::string& sFile, GLint internalFormat=GL_RGB );
	void								setTexture( osg::Texture* pTex, sigma::uint32 unit=0, const CSulString& uniformName="" );
	void								setTexture( osg::Image* pImage, GLint internalFormat=GL_RGB, sigma::uint32 unit=0 );

private:
	void								initConstructor();

private:
	osg::ref_ptr<CSulGeomQuad>			m_rGeomQuad;
	osg::ref_ptr<CSulGeode>				m_geodeQuad;
	osg::ref_ptr<osg::Group>			m_rGroup;
	osg::ref_ptr<osg::MatrixTransform>	m_rMT;
};

#endif // __SULSCREENALIGNEDQUAD_H__