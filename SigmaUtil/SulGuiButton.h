// SulGuiButton.h

#ifndef __SULGUIBUTTON_H__
#define __SULGUIBUTTON_H__

#include "SulGuiComp.h"

class CSulGuiButton : public CSulGuiComp
{
public:
	CSulGuiButton( const CSulString& sName, float x, float y, float w, float h );

	void	eventMouseMove( float mouse_x, float mouse_y );

private:
	class CSulGuiText* test;
};

#endif // __SULGUIBUTTON_H__