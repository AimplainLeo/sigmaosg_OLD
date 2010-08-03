// SulGuiCanvas.h

#ifndef __SULGUICANVAS_H__
#define __SULGUICANVAS_H__

#include "SulGuiComp.h"

class CSulGuiCanvas : public CSulGuiComp
{
public:
					CSulGuiCanvas( float x, float y, float w, float h );

	void			showCanvas( bool bShow );

	void			setBgColor( const osg::Vec4& c );
	void			setBorderColor( const osg::Vec4& c );

	float			getW();
	float			getH();

	virtual void	eventMouseMove( float local_x, float local_y, float x, float y );
	virtual void	eventMouseRelease( float x_local, float y_local, float x, float y );

	virtual void	setMouseHover( bool bHover );
	virtual void	setMouseRelease( bool bInside );

private:
	osg::ref_ptr<CSulGeomQuad>	m_rQuad;
	osg::ref_ptr<osg::Geode>	m_rGeodeQuad;
	osg::ref_ptr<osg::Uniform>	m_uniformBgColor;
	osg::ref_ptr<osg::Uniform>	m_uniformBorderColor;

	float						m_w;
	float						m_h;
	bool						m_bMouseHover;
};

#endif // __SULGUICANVAS_H__