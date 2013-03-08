// particleBase.h

#ifndef __PARTICLEBASE_H__
#define __PARTICLEBASE_H__

#include "propertySheet.h"

class CParticleBase
{
public:
	CParticleBase();

	CPropertySheet* getPropertySheet();
	
	virtual osg::Node* create( const osg::Vec3& pos );

protected:
	virtual CPropertySheet* createPropertySheet() = 0;

private:
	CPropertySheet* m_propertySheet;
};

#endif // __PARTICLEBASE_H__