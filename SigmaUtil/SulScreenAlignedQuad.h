// SulScreenAlignedQuad.h

#ifndef __SULSCREENALIGNEDQUAD_H__
#define __SULSCREENALIGNEDQUAD_H__

#include "SulExport.h"
#include "SulTypes.h"
#include "SulGeomQuad.h"
#include <osg/Referenced>
#include <osg/Group>

class SUL_EXPORT CSulScreenAlignedQuad : public osg::Referenced
{
public:
										CSulScreenAlignedQuad( const osg::Vec3& vPos, float w, float h, float fViewW, float fViewH );

	osg::Projection*					getProjection();
	osg::Group*							getGroup();
	CSulGeomQuad*						getQuad();

	void								setTexture( const std::string& sFile, GLint internalFormat=GL_RGB );
	void								setTexture( osg::Texture* pTex );
	void								setTexture( osg::Image* pImage, GLint internalFormat=GL_RGB );

private:
	osg::ref_ptr<CSulGeomQuad>			m_rGeomQuad;
	osg::ref_ptr<osg::Group>			m_rGroup;
	osg::ref_ptr<osg::MatrixTransform>	m_rMT;
	osg::ref_ptr<osg::Projection>		m_rProj;
};

#endif // __SULSCREENALIGNEDQUAD_H__