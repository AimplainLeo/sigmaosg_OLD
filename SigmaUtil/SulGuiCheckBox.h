// SulGuiCheckBox.h

#ifndef __SULGUICHECKBOX_H__
#define __SULGUICHECKBOX_H__

#include "SulGuiCanvas.h"
#include "SulExport.h"
#include <osg/group>

class SUL_EXPORT CSulGuiCheckBox : public CSulGuiCanvas
{
public:
								CSulGuiCheckBox( const CSulString& sText, float x, float y, float w, float h );

	virtual void				setupAttr( CSulXmlAttr* pAttr );
	virtual void				setupEventHandler( class CSulGuiEventHandler* pEventHandler );

	void						init();

	bool	isChecked();

	virtual class CSulGuiCheckBox*		asCheckBox()	{ return this; }

protected:
	void						setMouseRelease( bool bInside );

private:
	osg::ref_ptr<osg::Group>	m_rGroupCross;
	bool						m_bActive;
	float						m_paddingCross;
	float						m_paddingText;
	CSulString					m_sText;
	float						m_fontSize;
	float						m_boxSize;
};

#endif // __SULGUICHECKBOX_H__