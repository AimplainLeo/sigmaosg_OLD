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
	// FIXME: should use the OSG events instead
	enum EEVENTS
	{
		EVENT_MOUSE_MOVE		= 1,
		EVENT_MOUSE_PUSHED		= 2,
		EVENT_MOUSE_RELEASE		= 4,
		EVENT_KEYDOWN			= 4,
	};

public:
					CSulGuiComp( float x, float y );

	void			setXY( float x, float y );
	float			getX();
	float			getY();

	Sigma::uint32	getEvents();

	void			setLayer( Sigma::uint32 layer );

	virtual void	eventMouseMove( float mouse_local_x, float mouse_local_y, float mouse_x, float mouse_y );
	virtual void	eventMouseRelease( float x_local, float y_local, float x, float y );
	virtual void	eventKeyDown( Sigma::int32 key, Sigma::int32 iMod );

protected:
	void			addEvents( Sigma::uint32 events );

private:
	Sigma::uint32	m_events;
};

typedef std::vector< osg::ref_ptr<CSulGuiComp > >	VEC_GUICOMP;

#endif // SULGUICOMP_H__