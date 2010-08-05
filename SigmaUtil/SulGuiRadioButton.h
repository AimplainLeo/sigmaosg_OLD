// SulGuiRadioButton.h

#ifndef __SULGUIRADIOBUTTON_H__
#define __SULGUIRADIOBUTTON_H__

#include "SulGuiCanvas.h"

class CSulGuiRadioButton : public CSulGuiCanvas
{
public:
								CSulGuiRadioButton( const CSulString& sText, float x, float y, float w, float h );

	void						setupEventHandler( CSulGuiEventHandler* pEventHandler );

	void						removeSelect();

protected:
	void						setMouseRelease( bool bInside );

private:
	osg::ref_ptr<osg::Geode>	m_rGeodeCross;
	bool						m_bActive;
	osg::ref_ptr<osg::Geode>	m_rGeodeTriangleFan;
};

typedef std::vector< osg::ref_ptr<CSulGuiRadioButton> >	VEC_RADIOBUTTON;

#endif // __SULGUIRADIOBUTTON_H__