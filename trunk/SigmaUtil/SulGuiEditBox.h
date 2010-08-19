// SulGuiEditBox.h

#ifndef __SULGUIEDITBOX_H__
#define __SULGUIEDITBOX_H__

#include "SulGuiCanvas.h"
#include "SulGuiText.h"

class CSulGuiEditBox : public CSulGuiCanvas
{
public:
						CSulGuiEditBox( const CSulString& sText, float x, float y, float w, float h, float fontSize );

	virtual void		init();

	virtual void		setupAttr( CSulXmlAttr* pAttr );
	virtual void		setupEventHandler( class CSulGuiEventHandler* pEventHandler );

	void				setText( const CSulString& sText );
	const CSulString&	getText() const;

	void				setCursor( const CSulString& sCursor );

protected:
	virtual void		setMouseRelease( bool bInside );

private:
	void				onKeyDown( sigma::int32 key, sigma::int32 iMod );

private:
	osg::ref_ptr<CSulGuiText>	m_rGuiText;
	CSulString					m_sCursor;
	bool						m_bEditActive;
	CSulString					m_sText;
	float						m_fontSize;
	float						m_ofs_x;
	float						m_ofs_y;
};

#endif // __SULGUIEDITBOX_H__