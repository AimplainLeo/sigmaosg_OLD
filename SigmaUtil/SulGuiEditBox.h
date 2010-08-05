// SulGuiEditBox.h

#ifndef __SULGUIEDITBOX_H__
#define __SULGUIEDITBOX_H__

#include "SulGuiCanvas.h"
#include "SulGuiText.h"

class CSulGuiEditBox : public CSulGuiCanvas
{
public:
					CSulGuiEditBox( const CSulString& sText, float x, float y, float w, float h );

	void			setupEventHandler( class CSulGuiEventHandler* pEventHandler );

	void			setText( const CSulString& sText );
	const CSulString& getText() const;

	void			setCursor( const CSulString& sCursor );

	void			eventKeyDown( Sigma::int32 key, Sigma::int32 iMod );

protected:
	virtual void	setMouseRelease( bool bInside );

private:
	osg::ref_ptr<CSulGuiText>	m_rGuiText;
	CSulString					m_sCursor;
	bool						m_bActive;
	CSulString					m_sText;
};

#endif // __SULGUIEDITBOX_H__