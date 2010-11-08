// SulGuiDial.h

#ifndef __SULGUIDIAL_H__
#define __SULGUIDIAL_H__

#include "SulGuiCanvas.h"

class SUL_EXPORT CSulGuiDial : public CSulGuiCanvas
{
public:
					CSulGuiDial();
					CSulGuiDial( float radius );

	virtual void	setupAttr( CSulXmlAttr* pAttr );

	virtual void	init();

	osg::Vec2		getDir();

	virtual class CSulGuiDial*			asDial()		{ return this; }

private:
	virtual void eventMouseRelease( class CSulGuiCanvas* pCanvas, float local_x, float local_y, float x, float y );

private:
	float									m_radius;
	float									m_deg;
	osg::ref_ptr< osg::MatrixTransform >	m_mt;
};

#endif // __SULGUIDIAL_H__