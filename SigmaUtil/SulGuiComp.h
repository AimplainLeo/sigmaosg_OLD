// SulGuiComp.h

#ifndef SULGUICOMP_H__
#define SULGUICOMP_H__

#include "SulString.h"
#include "SulGeomQuad.h"
#include "SulSigSlots.h"
#include "SulGuiThemeXml.h"
#include "SulSigSlots.h"
#include "SulExport.h"
#include <osg/matrixtransform>
#include <osg/geode>
#include <stack>

class SUL_EXPORT CSulGuiComp : public osg::MatrixTransform, public sigma::has_slots<>
{
public:
					CSulGuiComp( const CSulString& sCompName );
					CSulGuiComp( const CSulString& sCompName, float x, float y );

	// must be called after all setups have been called
	virtual void	init();

	virtual bool	addChild( Node *child );

	const CSulString& getCompName() const;

	virtual void	setEditMode( bool bEdit );
	bool			isEditMode();

	// active buttons can still be moved and clicked on, but depending on the component it should limit what is active
	void			setActive( bool bActive );
	bool			isActive();

	virtual void	setupAttr( CSulXmlAttr* pAttr );
	virtual void	setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual void	setupEventHandler( class CSulGuiEventHandler* pEventHandler );

	CSulGuiThemeXml* getTheme();

	void			setXY( float x, float y );
	void			setX( float x );
	void			setY( float y );
	float			getX();
	float			getY();
	float			getWorldX();
	float			getWorldY();

	osg::Vec2		getLocal( const osg::Vec2& vWorld );
	osg::Vec2		getLocal( float xWorld, float yWorld );

	virtual void	show( bool bShow );
	virtual void	toggleShow();
	bool			isVisible();

	virtual void	setLayer( sigma::uint32 layer );
	sigma::uint32	getLayer();

	// deprecated event system
	virtual void	eventMouseMove( float local_x, float local_y, float x, float y );
	virtual void	eventMouseDrag( float local_x, float local_y, float x, float y );
	virtual void	eventMousePushed( float local_x, float local_y, float x, float y );
	virtual void	eventMouseRelease( float x_local, float y_local, float x, float y );
	virtual void	eventKeyDown( sigma::int32 key, sigma::int32 iMod );

	// new event system
	virtual bool	eventMousePush( class CSulGuiCanvas* pCanvas, float local_x, float local_y, float x, float y ) { return false; }
	virtual bool	eventMouseRelease( class CSulGuiCanvas* pCanvas, float local_x, float local_y, float x, float y ) { return false; }

	CSulString						getThemeValue( const CSulString& attr );
	CSulGuiEventHandler*			getEventHandler();
	osg::Vec2						getNativeDimensions();
	const CSulString&				getId() const;

	virtual class CSulGuiCanvas*		asCanvas()		{ return 0; }
	virtual class CSulGuiTextBox*		asTextBox()		{ return 0; }
	virtual class CSulGuiListBox*		asListBox()		{ return 0; }
	virtual class CSulGuiButton*		asButton()		{ return 0; }
	virtual class CSulGuiEditBox*		asEditBox()		{ return 0; }
	virtual class CSulGuiRadioButton*	asRadioButton()	{ return 0; }
	virtual class CSulGuiComboBox*		asComboBox()	{ return 0; }
	virtual class CSulGuiCheckBox*		asCheckBox()	{ return 0; }
	virtual class CSulGuiDial*			asDial()		{ return 0; }
	virtual class CSulGuiProfiler*		asProfiler()	{ return 0; }
	virtual class CSulGuiAlign*			asAlign()		{ return 0; }
	virtual class CSulGuiText*			asText()		{ return 0; }
	virtual class CSulGuiSliderH*		asSliderH()		{ return 0; }
	virtual class CSulGuiTable*			asTable()		{ return 0; }
	virtual class CSulGuiListView*		asListView()	{ return 0; }

	sigma::signal2<float, float>	signalPositionChanged;

private:
	void			initConstructor();

	void			onNativeDimensionsChanged( float w, float h );

private:
	class CSulGuiEventHandler*		m_pEventHandler;
	bool							m_bActive;
	osg::ref_ptr<CSulGuiThemeXml>	m_rThemeXml;
	CSulString						m_sCompName;
	osg::Vec2						m_nativeDimensions;
	CSulString						m_id;

	bool							m_bEditMode;

	sigma::uint32					m_renderbinNum;

	float							m_x;			
	float							m_y;	

	sigma::uint32					m_layer;

	bool							m_bShow;
	osg::Group*						m_pParentLast; // used for method show
};

typedef std::vector< osg::ref_ptr<CSulGuiComp > >	VEC_GUICOMP;
typedef std::stack< osg::ref_ptr<CSulGuiComp > >	STACK_GUICOMP;

#endif // SULGUICOMP_H__