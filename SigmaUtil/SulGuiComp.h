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
					CSulGuiComp( float x, float y );

	virtual void	setupEventHandler( class CSulGuiEventHandler* pEventHandler );

	void			setXY( float x, float y );
	void			setY( float y );
	float			getX();
	float			getY();

	void			show( bool bShow );

	void			setLayer( Sigma::uint32 layer );

	virtual void	eventMouseMove( float mouse_local_x, float mouse_local_y, float mouse_x, float mouse_y );
	virtual void	eventMouseRelease( float x_local, float y_local, float x, float y );
	virtual void	eventKeyDown( Sigma::int32 key, Sigma::int32 iMod );

protected:
	void			addEvent( Sigma::uint32 eventType );

private:
	class CSulGuiEventHandler*	m_pEventHandler;
};

typedef std::vector< osg::ref_ptr<CSulGuiComp > >	VEC_GUICOMP;

#endif // SULGUICOMP_H__