// SulGuiSliderH.h

#ifndef __SULGUISLIDERH_H__
#define __SULGUISLIDERH_H__

#include "SulGuiCanvas.h"
#include "SulExport.h"

class SUL_EXPORT CSulGuiSliderH : public CSulGuiCanvas
{
public:
					CSulGuiSliderH();

	virtual void	setupAttr( CSulXmlAttr* pAttr );
	virtual void	init();

	void			setValue( float v );
	float			getValue();

	CSulGuiButton*	getSlider();

	virtual class CSulGuiSliderH*	asSliderH()		{ return this; }

private:
	void			onSliderChanged( float x, float y );

private:
	float							m_size;
	float							m_value;
	osg::ref_ptr<CSulGuiButton>		m_rSlider;
};

#endif // __SULGUISLIDERH_H__