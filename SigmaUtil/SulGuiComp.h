// SulGuiComp.h

#ifndef SULGUICOMP_H__
#define SULGUICOMP_H__

#include "SulString.h"
#include "SulGeomQuad.h"
#include <osg/matrixtransform>
#include <osg/geode>

class CSulGuiComp : public osg::MatrixTransform
{
public:
	enum EEVENTS
	{
		EVENT_MOUSE_MOVE		= 1,
		EVENT_MOUSE_PUSHED		= 2,
		EVENT_MOUSE_RELEASED	= 4
	};

public:
				CSulGuiComp( const CSulString& sName, float x, float y, float w, float h );

	void			setQuadColor( osg::Vec4& c );
	void			setRenderQuad( bool bRenderQuad );

	float			getX();
	float			getY();

	Sigma::uint32	getEvents();

	virtual void	eventMouseMove( float mouse_x, float mouse_y ) {}

protected:
	void			addEvents( Sigma::uint32 events );

private:
	CSulString	m_sName;
	float		m_w;
	float		m_h;

	osg::ref_ptr<CSulGeomQuad>			m_rQuad;
	osg::Vec4							m_rQuadColor;
	osg::ref_ptr<osg::Geode>			m_rGeodeQuad;
	Sigma::uint32						m_events;
};

typedef std::vector< osg::ref_ptr<CSulGuiComp > >	VEC_GUICOMP;

#endif // SULGUICOMP_H__