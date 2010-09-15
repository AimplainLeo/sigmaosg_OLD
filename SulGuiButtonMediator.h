// SulGuiButtonMediator.h

#ifndef __SULGUIBUTTONMEDIATOR_H__
#define __SULGUIBUTTONMEDIATOR_H__

//#include "SulGuiButton.h"

class CSulGuiButtonMediatorBase
{
public:
	virtual void click( class CSulGuiButton* p ) {}
};

template<class TClass>
class CSulGuiButtonMediator : public CSulGuiButtonMediatorBase
{
public:
	CSulGuiButtonMediator( TClass* pClass )
	{
		m_pClass = pClass;
	}

	void click( CSulGuiButton* p )
	{
		m_pClass->buttonClick( p );
	}

private:
	TClass* m_pClass;
};

#endif // __SULGUIBUTTONMEDIATOR_H__