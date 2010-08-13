// SulGuiScrollBar.h

#ifndef __SULGUISCROLLBAR_H__
#define __SULGUISCROLLBAR_H__

#include "SulGuiCanvas.h"
#include "SulGuiButton.h"
#include "SulSigSlots.h"

class CSulGuiScrollBar : public CSulGuiCanvas, public sigma::has_slots<>
{
public:
			CSulGuiScrollBar( float x, float y, float w, float h );

	virtual void	setupEventHandler( CSulGuiEventHandler* pEventHandler );

	virtual void	init();

	void	buttonClick( CSulGuiButton* pButton );

	sigma::signal1<float>	signalChanged;

private:
	void	onButtonDragPositionChanged( float x, float y );

private:
	osg::ref_ptr<CSulGuiButton>		m_rButtonTop;
	osg::ref_ptr<CSulGuiButton>		m_rButtonBottom;
	osg::ref_ptr<CSulGuiButton>		m_rButtonDrag;

	float							m_rangeDrag;		// pixel drag range
	float							m_min;
	float							m_max;
	float							m_cur;
};

#endif // __SULGUISCROLLBAR_H__