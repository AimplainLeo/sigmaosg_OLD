// SulScreenAligned.h

#ifndef __SULSCREENALIGNED_H__
#define __SULSCREENALIGNED_H__

#include "SulTypes.h"
#include "SulExport.h"
#include <osg/projection>

class SUL_EXPORT CSulScreenAligned : public osg::Projection
{
public:
	enum EALIGNMENT
	{
		BOTTOM_LEFT,			
		CENTER_CENTER		// default
	};

public:
								CSulScreenAligned( sigma::uint32 viewW, sigma::uint32 viewH  );

	// add objects to this transform
	osg::MatrixTransform*		getMatrixTransform();

	void						setPosition( const osg::Vec3& pos );

private:
	void initConstructor();

private:
	osg::ref_ptr<osg::MatrixTransform>	m_mt;
	sigma::uint32						m_viewW;
	sigma::uint32						m_viewH;
	EALIGNMENT							m_alignment;
};

#endif // __SULSCREENALIGNED_H__