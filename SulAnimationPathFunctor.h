// SulAnimationPathFunctor.h

#ifndef __SULANIMATIONPATHFUNCTOR_H__
#define __SULANIMATIONPATHFUNCTOR_H__

#include <osg/Referenced>

class CSulAnimationPathFunctorBase : public osg::Referenced
{
public:
	virtual void Call() {}
};

template <class TClass>
class CSulAnimationPathFunctor : public CSulAnimationPathFunctorBase
{
public:
	CSulAnimationPathFunctor( TClass* pObject, void(TClass::*pFunction)() )
	{
		m_pObj = pObject;
		m_pFunction = pFunction;
	}

	virtual void Call()
	{
		(*m_pObj.*m_pFunction)();
	}

private:
	TClass* m_pObj;
	void (TClass::*m_pFunction)();
};

#endif // __SULANIMATIONPATHFUNCTOR_H__