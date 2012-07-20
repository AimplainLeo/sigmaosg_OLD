// SulClouds.h

#ifndef __SULCLOUDS_H__
#define __SULCLOUDS_H__

#include "SulCloudPlane.h"
#include "SulTexImage.h"
#include "SulExport.h"
#include "SulPerlinNoise2D.h"
#include <osg/node>
#include <osg/referenced>

class SUL_EXPORT CSulClouds : public osg::Referenced
{
public:
				CSulClouds( CSulCloudPlane* pCloudPlane=0 );

	// coverage = 0.0-1.0
	void		setCoverage( float coverage );
	float		getCoverage();

	// not used right now, in the original they modify the fog color used on the clouds)
	void		setColor( const osg::Vec4& color );

	void		setWind( float x, float y );
		
	void		setHeight( float h );
	float		getHeight();

	void		setSize( float s );
	float		getSize();

	void		setUV( float s );

	osg::Node*	getNode();
	osg::Group*	getGroup();

	// must call this before create if you have a custom cloudplane
	void		setCloudPlane( CSulCloudPlane* pCloudPlane );

	// work in progress,. used by perlin noise
	CSulTexImage* getTexImage();
	void			setOctave( float o );
	void			setPersistence( float p );
	void			setScale( float s );
	void			setOfs( float o );
	void			generate();

	osg::Image*		getImage();

	CSulCloudPlane*	getCloudPlane();

private:
	void		setTextureStates();

	osg::Node*	create();
	osg::Node*	createPlane( float size, float height );

private:
	// work in progress for self perlin noise cloud generation
	osg::ref_ptr<CSulTexImage> m_rTexImage;
	osg::ref_ptr<osg::Image>	m_rImage;
	float*						m_pData;

	osg::ref_ptr<CSulPerlinNoise2D> m_rPerlin;
	
	osg::ref_ptr<CSulCloudPlane>					m_rPlane;
	osg::ref_ptr<osg::PositionAttitudeTransform>	m_rPat;
	float											m_coverage;
	float											m_size;
	float											m_height;
	osg::Vec2										m_wind;
	osg::ref_ptr<osg::Group>						m_rGroup;
};

#endif // __SULCLOUDS_H__
