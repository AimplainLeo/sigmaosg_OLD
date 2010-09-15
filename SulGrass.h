// SulGrass.h

#ifndef __SULGRASS_H__
#define __SULGRASS_H__

#include "SulExport.h"
#include "SulTypes.h"
#include <osg/geode>
#include <osg/referenced>
#include <osg/texture2d>

class SUL_EXPORT CSulGrass : public osg::Referenced
{
public:
								CSulGrass();

	void						setReceiveShadow( bool bReceiveShadow );

	void						setTextureUnit_HeightMap( sigma::int32 i );

	void						setShowBB( bool bShow );

	osg::Shader*				getShaderVert();
	osg::Shader*				getShaderFrag();

	void						setSizeMultiplier( float sMul );
	
	void						setTextureFileName( const std::string& s );

	void						setPosition( const osg::Vec3& pos );

	// FIXME: spacing and grid size are depend on each other.. need to make this easier to understand
	void						setSpacing( float spacing );

	void							setGridSize( sigma::uint32 size );
	sigma::uint32					getGridSize();

	osg::PositionAttitudeTransform*	getPat();
	osg::Geode*						getGeode();
	osg::BoundingBox*				getBoundingBox();

	void							setHeightTexture( osg::Texture2D* pTex );
	
private:
	void						create();
	void						createGeometry( osg::Geometry& geom, int nInstances=1 );
	void						createShader();

private:
	osg::ref_ptr<osg::Geode>	m_geodeGrass;
	sigma::uint32				m_sizeGrid;		// m_sizeGrid=2 => 2 x 2 grid
	float						m_spacing;
	float						m_heightAdjust;	// tweak the height position of the grass (default: 0)
	float						m_windFactor;	// defines how much movement there is in the grass (no direction)
	float						m_grassStretch;	// how much the grass should be stretched
	osg::Vec3					m_vPos;
	float						m_sizeMultiplier;

	osg::ref_ptr<osg::Uniform>	uniformSpacing;
	osg::ref_ptr<osg::Uniform>	uniformSizeGrid;
	osg::ref_ptr<osg::Uniform>	uniformLock;
	osg::ref_ptr<osg::Uniform>	uniformHeightAdjust;
	osg::ref_ptr<osg::Uniform>	uniformWindFactor;
	osg::ref_ptr<osg::Uniform>	uniformGrassStretch;
	osg::ref_ptr<osg::Uniform>	uniformHeightMap;
	osg::ref_ptr<osg::Uniform>	uniformReceiveShadow;

	osg::ref_ptr<osg::StateSet>		m_ss;
	osg::BoundingBox*				m_bb;

	std::string						m_sTextureGrass;

	osg::ref_ptr<osg::Shader>						m_shaderVert;
	osg::ref_ptr<osg::Shader>						m_shaderFrag;

	osg::ref_ptr<osg::PositionAttitudeTransform>	m_rPat;

	bool	m_bShowBB;

	sigma::int32									m_iTexUnit_HeightMap;
	bool											m_bReceiveShadow;
};

#endif // __SULGRASS_H__