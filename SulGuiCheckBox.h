// SulGuiCheckBox.h

#ifndef __SULGUICHECKBOX_H__
#define __SULGUICHECKBOX_H__

#include "SulGuiCanvas.h"

class CSulGuiCheckBox : public CSulGuiCanvas
{
public:
								CSulGuiCheckBox( const CSulString& sText, float x, float y, float w, float h );

	virtual void				setupAttr( CSulXmlAttr* pAttr );
	virtual void				setupEventHandler( class CSulGuiEventHandler* pEventHandler );

	void						init();

protected:
	void						setMouseRelease( bool bInside );

private:
	osg::ref_ptr<osg::Geode>	m_rGeodeCross;
	bool						m_bActive;
	float						m_paddingCross;
	float						m_paddingText;
	CSulString					m_sText;
	float						m_fontSize;
	float						m_boxSize;
};

#endif // __SULGUICHECKBOX_H__