// SulConsoleFunctor.h

#ifndef __SULCONSOLEFUNCTOR_H__
#define __SULCONSOLEFUNCTOR_H__

#include <osg/Referenced>

class CSulConsoleFunctorBase : public osg::Referenced
{
public:
	virtual void Call( const class CSulString& sCmd ) {}
};

template <class TClass>
class CSulConsoleFunctor : public CSulConsoleFunctorBase
{
public:
	CSulConsoleFunctor( TClass* pObject, void(TClass::*pFunction)(const class CSulString& sCmd) )
	{
		m_pObj = pObject;
		m_pFunction = pFunction;
	}

	virtual void Call( const class CSulString& sCmd )
	{
		(*m_pObj.*m_pFunction)( sCmd );
	}

private:
	TClass* m_pObj;
	void (TClass::*m_pFunction)(const class CSulString& sCmd);
};

#endif // __SULCONSOLEFUNCTOR_H__