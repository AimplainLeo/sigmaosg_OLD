// SulShadowTextureNodeCallback.h

#ifndef __SULSHADOWTEXTURENODECALLBACK_H__
#define __SULSHADOWTEXTURENODECALLBACK_H__

#include <osg/texgennode>
#include <osg/camera>

class CSulShadowTextureNodeCallback : public osg::NodeCallback
{
public:
	CSulShadowTextureNodeCallback( osg::Node* pBounds, osg::Node* pRenderMe, osg::Camera* pCam, osg::TexGenNode* pTexGenNode, const osg::Vec3& posLight );

	void setLight( const osg::Vec3& pos );

    virtual void operator()( osg::Node* node, osg::NodeVisitor* nv );

private:
	osg::ref_ptr<osg::Node>			m_rRenderMe;
	osg::ref_ptr<osg::Camera>		m_rCam;
	osg::ref_ptr<osg::TexGenNode>	m_rTexGenNode;
	osg::ref_ptr<osg::Node>			m_rBounds;
	osg::Vec3						m_rPosLight;
};

#endif // __SULSHADOWTEXTURENODECALLBACK_H__