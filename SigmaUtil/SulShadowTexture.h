// SulShadowTexture.h

#ifndef __SULSHADOWTEXTURE_H__
#define __SULSHADOWTEXTURE_H__

#include "SulRTT.h"
#include "SulTypes.h"
#include "SulShadowTextureNodeCallback.h"
#include <osg/node>
#include <osg/texgennode>
#include <osg/referenced>

class CSulShadowTexture : public osg::Referenced
{
public:
						CSulShadowTexture( osg::Node* pRenderMe, osg::TexGenNode* pTexGenNode );

	void				init();
	
	void				setShadowMapBounds( osg::Node* pNode );

	void				setTextureSize( sigma::uint32 size_squared );
	void				setLight( const osg::Vec3& pos );

	osg::Node*			getNode();
	osg::Camera*		getCamera();
	osg::Texture2D*		getTexture();

private:
	osg::ref_ptr<osg::Node>			m_rRenderMe;
	osg::Vec3						m_pos;
	osg::ref_ptr<CSulRTT>			m_rRTT;
	osg::ref_ptr<osg::TexGenNode>	m_rTexGenNode;
	osg::ref_ptr<osg::Group>		m_rGroup;
	osg::ref_ptr<osg::Node>			m_rShadowMapBounds;
	sigma::uint32					m_size_squared;

	osg::ref_ptr<CSulShadowTextureNodeCallback>	m_rShadowTextureNodeCallback;
};

#endif // __SULSHADOWTEXTURE_H__