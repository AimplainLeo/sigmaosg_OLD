// SulRenderInstances.h

#ifndef __SULRENDERINSTANCES_H__
#define __SULRENDERINSTANCES_H__

#include "SulTypes.h"
#include <osg/group>
#include <osg/texture2d>
#include <osg/copyop>

class CSulRenderInstances : public osg::Group
{
public:
				CSulRenderInstances();

				CSulRenderInstances( osg::Image* pImagePositions, sigma::uint32 numInst, osg::BoundingBox& bb, float min=3.0f, float max=3.0f, bool bSuppressTexture=false, sigma::uint32 texUnit=0, sigma::uint32 texSizeSquared=256, sigma::uint32 useLights=0x00000001, bool bUseZDirectionNormal=false, bool bSuppressShaders=false );

				CSulRenderInstances( const CSulRenderInstances& ri, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY );

	sigma::uint32			getNumInstances() const;
	const osg::BoundingBox&	getBB() const;
	osg::Texture2D*			getTexturePositions() const;

	void		create();

protected:
	virtual void createShaders();

private:
	osg::ref_ptr<osg::Texture2D>	m_rTexturePositions;
	sigma::uint32					m_numInstances;
	osg::BoundingBox				m_bb;
	bool							m_bSuppressTexture;
	sigma::uint32					m_texUnit;
	sigma::uint32					m_texSizeSquared;
	sigma::uint32					m_useLights;
	float							m_min;
	float							m_max;
	bool							m_bUseZDirectionNormal;
	bool							m_bSuppressShaders;
};

#endif // __SULRENDERINSTANCES_H__