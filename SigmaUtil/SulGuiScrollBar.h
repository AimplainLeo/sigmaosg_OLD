// SulGuiScrollBar.h

#ifndef __SULGUISCROLLBAR_H__
#define __SULGUISCROLLBAR_H__

#include "SulGuiCanvas.h"
#include "SulGuiButton.h"

class CSulGuiScrollBar : public CSulGuiCanvas
{
public:
			CSulGuiScrollBar( float x, float y, float w, float h );

	void	setupEventHandler( CSulGuiEventHandler* pEventHandler );

	void	buttonClick( CSulGuiButton* pButton );

private:
	osg::ref_ptr<CSulGuiButton>		m_rButtonTop;
	osg::ref_ptr<CSulGuiButton>		m_rButtonBottom;
	osg::ref_ptr<CSulGuiButton>		m_rButtonDrag;
};

#endif // __SULGUISCROLLBAR_H__