// SulGuiCanvas.h

#ifndef __SULGUICANVAS_H__
#define __SULGUICANVAS_H__

#include "SulGuiComp.h"
#include "SulSigSlots.h"

class CSulGuiCanvas : public CSulGuiComp
{
public:
					CSulGuiCanvas( const CSulString& sCompName, float x, float y, float w, float h );

	virtual void	init();

	virtual void	setupAttr( CSulXmlAttr* pAttr );
	virtual void	setupEventHandler( CSulGuiEventHandler* pEventHandler );

	void			setDraggable( bool bDraggable );

	void			showCanvas( bool bShow );

	void			setBgColor( const osg::Vec4& c );
	void			setBorderColor( const osg::Vec4& c );

	bool			isInside( float x, float y );

	float			getW();
	float			getH();

	void			allowDrag( float minX, float maxX, float minY, float maxY );

	virtual void	eventMouseMove( float local_x, float local_y, float x, float y );
	virtual void	eventMouseDrag( float local_x, float local_y, float x, float y );
	virtual void	eventMousePushed( float x_local, float y_local, float x, float y );
	virtual void	eventMouseRelease( float x_local, float y_local, float x, float y );

	virtual void	setMouseHover( bool bHover );
	virtual void	setMouseRelease( bool bInside );

	sigma::signal1<CSulGuiCanvas*>			signalClicked;

public:
osg::ref_ptr<osg::Geode>	m_rGeodeQuad;

private:
	osg::ref_ptr<CSulGeomQuad>	m_rQuad;
	
	osg::ref_ptr<osg::Uniform>	m_uniformBgColor;
	osg::ref_ptr<osg::Uniform>	m_uniformBorderColor;

	float						m_w;
	float						m_h;
	bool						m_bMouseHover;

	bool						m_dragAllowed;		// flag to indicate that the canvas can be dragged
	float						m_dragMinX;			// minimum drag x axis
	float						m_dragMaxX;			// maximum drag x axis
	float						m_dragMinY;			// minimum drag y axis
	float						m_dragMaxY;			// maximum drag y axis
	osg::Vec2					m_dragOfsPos;		// drag offset start position mouse
	bool						m_dragDragging;		// dragging is in process

};

typedef std::vector< osg::ref_ptr<CSulGuiCanvas> >	VEC_GUICANVAS;

#endif // __SULGUICANVAS_H__