// SulRenderInstances.h

#ifndef __SULRENDERINSTANCES_H__
#define __SULRENDERINSTANCES_H__

#include "SulTypes.h"
#include <osg/geode>
#include <osg/texture2d>
#include <osg/copyop>

class CSulRenderInstances : public osg::Geode
{
public:
				CSulRenderInstances();

				CSulRenderInstances( osg::Image* pImagePositions, Sigma::uint32 numInst, osg::BoundingBox& bb, float min=3.0f, float max=3.0f, bool bSuppressTexture=false, Sigma::uint32 texUnit=0, Sigma::uint32 texSizeSquared=256, Sigma::uint32 useLights=0x00000001 );

				CSulRenderInstances( const CSulRenderInstances& ri, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY );

	//			META_Node( , CSulRenderInstances );

	Sigma::uint32			getNumInstances() const;
	const osg::BoundingBox&	getBB() const;
	osg::Texture2D*			getTexturePositions() const;

	void		create();

protected:
	virtual void createShaders();

private:
	osg::ref_ptr<osg::Texture2D>	m_rTexturePositions;
	Sigma::uint32					m_numInstances;
	osg::BoundingBox				m_bb;
	bool							m_bSuppressTexture;
	Sigma::uint32					m_texUnit;
	Sigma::uint32					m_texSizeSquared;
	Sigma::uint32					m_useLights;
	float							m_min;
	float							m_max;
};

#endif // __SULRENDERINSTANCES_H__