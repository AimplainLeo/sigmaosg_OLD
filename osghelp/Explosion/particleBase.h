// particleBase.h

#ifndef __PARTICLEBASE_H__
#define __PARTICLEBASE_H__

#include <SigmaUtil/SulQtPropertySheet.h>

class CParticleBase
{
public:
	CParticleBase();

	CSulQtPropertySheet* getPropertySheet();
	
	virtual osg::Node* create( const osg::Vec3& pos );

protected:
	virtual CSulQtPropertySheet* createPropertySheet() = 0;

private:
	CSulQtPropertySheet* m_propertySheet;
};

#endif // __PARTICLEBASE_H__