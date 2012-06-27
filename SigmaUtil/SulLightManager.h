// SulLightManager.h

#ifndef __SULLIGHTMANAGER_H__
#define __SULLIGHTMANAGER_H__

#include "SulLightBase.h"
#include "SulLightPoint.h"
#include "SulLightSpot.h"
#include "SulTypes.h"
#include "SulExport.h"
#include <osg/texturerectangle>
#include <osg/group>

class SUL_EXPORT CSulLightManager : public osg::Group
{
public:
			CSulLightManager();

	void	addVisible( CSulLightPoint* pLight );
	void	addVisible( CSulLightSpot* pLight );

	const VEC_LIGHT_POINT& getLightPointList();
	const VEC_LIGHT_SPOT& getLightSpotList();

	void	buildTextureData();

	osg::Uniform*		getUniformTexLightData();
	osg::Uniform*		getUniformCountLightPoints();
	osg::Uniform*		getUniformCountLightSpots();

	osg::TextureRectangle*	getTextureData();

private:
	void	traverse( osg::NodeVisitor& nv );

	void	reset();

	void	createTextureData( sigma::uint32 size, sigma::uint32 sizeY );

private:
	osg::ref_ptr<osg::Image>			m_rImage;
	osg::ref_ptr<osg::TextureRectangle>	m_rTextureData;

	VEC_LIGHT_POINT						m_lightPoint;
	VEC_LIGHT_SPOT						m_lightSpot;

	osg::ref_ptr<osg::Uniform>			m_uniformTexLightPoint;
	osg::ref_ptr<osg::Uniform>			m_uniformCountLightPoints;
	osg::ref_ptr<osg::Uniform>			m_uniformCountLightSpots;

	sigma::int32						m_lastFrame;
	sigma::int32						m_countCameras;

private:
	typedef std::vector< osg::ref_ptr< osg::Image> >	VEC_IMAGE;
	typedef std::vector< osg::ref_ptr< osg::TextureRectangle > > VEC_TEXTURERECTANGLE;

	VEC_IMAGE							m_vecImage;
	VEC_TEXTURERECTANGLE				m_vecTextureRectangle;
};

#endif // __SULLIGHTMANAGER_H__