// SulGeomLineStrip.h

#ifndef __SULGEOMLINESTRIP_H__
#define __SULGEOMLINESTRIP_H__

#include "SulGeomBase.h"
#include "SulTypes.h"

class CSulGeomLineStrip : public CSulGeomBase
{
public: 
					CSulGeomLineStrip( const Sigma::VEC_VEC3& vecVector );

private:
	Sigma::VEC_VEC3 m_vecVector;
};


#endif // __SULGEOMLINESTRIP_H__