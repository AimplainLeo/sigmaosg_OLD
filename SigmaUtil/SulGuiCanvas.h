	// SulGuiCanvas.h

#ifndef __SULGUICANVAS_H__
#define __SULGUICANVAS_H__

#include "SulGuiComp.h"

class CSulGuiCanvas : public CSulGuiComp
{
public:
					CSulGuiCanvas( const CSulString& sCompName );
					CSulGuiCanvas( const CSulString& sCompName, float x, float y );
					CSulGuiCanvas( const CSulString& sCompName, float x, float y, float w, float h );

	virtual void	init();

	virtual void	setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual void	setupAttr( CSulXmlAttr* pAttr );
	virtual void	setupEventHandler( CSulGuiEventHandler* pEventHandler );

	void			setDraggable( bool bDraggable );

	void			showCanvas( bool bShow );

	void			setBgColor( const osg::Vec4& c );
	void			setBorderColor( const osg::Vec4& c );

	bool			isInside( float x, float y );

	void			setWH( float w, float h );
	void			setW( float w );
	void			setH( float h );

	float			getW();
	float			getH();

	void			allowDrag( float minX, float maxX, float minY, float maxY );

	virtual void	setMouseRelease( bool bInside );

	sigma::signal1<CSulGuiCanvas*>			signalClicked;
	sigma::signal1<bool>					signalHover;

private:
	void			initConstructor();

	void			onMouseMove( float x, float y );
	void			onMouseDrag( float x, float y );
	void			onMousePush( float x, float y );
	void			onMouseRelease( float x, float y );
	void			onViewResize( float w, float h );

public:
	osg::ref_ptr<osg::Geode>	m_rGeodeQuad;		// FIXME: should be private

private:
	osg::ref_ptr<CSulGeomQuad>	m_rQuad;
	
	osg::ref_ptr<osg::Uniform>	m_uniformBgColor;
	osg::ref_ptr<osg::Uniform>	m_uniformBorderColor;
	osg::ref_ptr<osg::Uniform>	m_uniformW;
	osg::ref_ptr<osg::Uniform>	m_uniformH;
	osg::ref_ptr<osg::Uniform>	m_uniformUseTexture;

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

	CSulString					m_img;

	bool						m_bShowCanvas;
};

typedef std::vector< osg::ref_ptr<CSulGuiCanvas> >	VEC_GUICANVAS;

#endif // __SULGUICANVAS_H__