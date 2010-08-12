// SulShadow.h

#ifndef __SULSHADOW_H__
#define __SULSHADOW_H__

#include "SulShadowTexture.h"
#include "SulExport.h"
#include <osg/node>
#include <osg/group>
#include <osg/referenced>

class SUL_EXPORT CSulShadow : public osg::TexGenNode
{
public:
				CSulShadow( osg::Group* pSceneRoot );

	void		init();

	void		setTextureSize( sigma::uint32 size_squared );

	void		useShaderVert( const std::string& file );
	void		useShaderFrag( const std::string& file );

	void		setLightPosition( const osg::Vec3& pos );

	void		addCaster( osg::Node* pNode );

	// bounding box of this node defines the area the shadowmap texture will be created
	void		setShadowMapBounds( osg::Node* pNode );

	osg::Texture2D*	getShadowMapTexture();

private:
	osg::ref_ptr<osg::Group>		m_rSceneRoot;
	osg::ref_ptr<CSulShadowTexture>	m_rST;
	osg::ref_ptr<osg::Group>		m_rCasters;
	osg::ref_ptr<osg::Node>			m_rShadowMapBounds;
	osg::Vec3						m_posLight;
	std::string						m_fileShaderFrag;
	std::string						m_fileShaderVert;
	sigma::uint32					m_size_squared;
};

#endif // __SULSHADOW_H__