// SulGuiComp.h

#ifndef SULGUICOMP_H__
#define SULGUICOMP_H__

#include "SulString.h"
#include "SulGeomQuad.h"
#include "SulSigSlots.h"
#include "SulGuiThemeXml.h"
#include <osg/matrixtransform>
#include <osg/geode>
#include <stack>

class CSulGuiComp : public osg::MatrixTransform
{
public:
					CSulGuiComp( const CSulString& sCompName );
					CSulGuiComp( const CSulString& sCompName, float x, float y );

	// must be called after all setups have been called
	virtual void	init();

	// active buttons can still be moved and clicked on, but depending on the component it should limit what is active
	void			setActive( bool bActive );
	bool			isActive();

	virtual void	setupAttr( CSulXmlAttr* pAttr );
	virtual void	setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual void	setupEventHandler( class CSulGuiEventHandler* pEventHandler );
	virtual void	setupView( float w, float h );

	void			setXY( float x, float y );
	void			setX( float x );
	void			setY( float y );
	float			getX();
	float			getY();
	float			getWorldX();
	float			getWorldY();

	virtual void	show( bool bShow );
	virtual void	toggleShow();

	void			setLayer( sigma::uint32 layer );

	virtual void	eventMouseMove( float local_x, float local_y, float x, float y );
	virtual void	eventMouseDrag( float local_x, float local_y, float x, float y );
	virtual void	eventMousePushed( float local_x, float local_y, float x, float y );
	virtual void	eventMouseRelease( float x_local, float y_local, float x, float y );
	virtual void	eventKeyDown( sigma::int32 key, sigma::int32 iMod );

	sigma::signal2<float, float>	signalPositionChanged;

	CSulString		getThemeValue( const CSulString& attr );

	CSulGuiEventHandler*	getEventHandler();

private:
	void			initConstructor();

private:
	class CSulGuiEventHandler*		m_pEventHandler;
	bool							m_bActive;
	osg::ref_ptr<CSulGuiThemeXml>	m_rThemeXml;
	CSulString						m_sCompName;
};

typedef std::vector< osg::ref_ptr<CSulGuiComp > >	VEC_GUICOMP;
typedef std::stack< osg::ref_ptr<CSulGuiComp > >	STACK_GUICOMP;

#endif // SULGUICOMP_H__