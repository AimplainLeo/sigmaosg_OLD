// SulGuiRadioButton.h

#ifndef __SULGUIRADIOBUTTON_H__
#define __SULGUIRADIOBUTTON_H__

#include "SulGuiCanvas.h"

class CSulGuiRadioButton : public CSulGuiCanvas
{
public:
								CSulGuiRadioButton( const CSulString& sText, float x, float y, float w, float h );

protected:
	void						setMouseRelease( bool bInside );

private:
	osg::ref_ptr<osg::Geode>	m_rGeodeCross;
	bool						m_bActive;
};

#endif // __SULGUIRADIOBUTTON_H__