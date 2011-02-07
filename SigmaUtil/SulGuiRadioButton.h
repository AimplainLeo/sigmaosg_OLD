// SulGuiRadioButton.h

#ifndef __SULGUIRADIOBUTTON_H__
#define __SULGUIRADIOBUTTON_H__

#include "SulGuiCanvas.h"
#include "SulGeomTriangleFan.h"

class CSulGuiRadioButton : public CSulGuiCanvas
{
public:
								CSulGuiRadioButton( const CSulString& sText, float x, float y, float w, float h );

	virtual void				setupAttr( CSulXmlAttr* pAttr );
	virtual void				setupEventHandler( CSulGuiEventHandler* pEventHandler );

	virtual void				init();

	void						removeSelect();

	virtual class CSulGuiRadioButton*	asRadioButton()	{ return this; }

protected:
	void						setMouseRelease( bool bInside );

private:
	bool						m_bActive;
	CSulString					m_sText;
	float						m_fontSize;
	float						m_radioSizeInner;
	float						m_radioSizeOuter;
	float						m_paddingText;
	osg::ref_ptr<CSulGeomTriangleFan> m_rTriangleFan;
};

typedef std::vector< osg::ref_ptr<CSulGuiRadioButton> >	VEC_RADIOBUTTON;

#endif // __SULGUIRADIOBUTTON_H__