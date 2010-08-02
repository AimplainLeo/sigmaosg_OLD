// SulGuiCheckBox.h

#ifndef __SULGUICHECKBOX_H__
#define __SULGUICHECKBOX_H__

#include "SulGuiCanvas.h"

class CSulGuiCheckBox : public CSulGuiCanvas
{
public:
								CSulGuiCheckBox( const CSulString& sText, float x, float y, float w, float h );

protected:
	void						setMouseRelease( bool bInside );

private:
	osg::ref_ptr<osg::Geode>	m_rGeodeCross;
	bool						m_bActive;
};

#endif // __SULGUICHECKBOX_H__