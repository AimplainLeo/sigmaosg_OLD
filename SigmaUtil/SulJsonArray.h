// SulJsonArray.h

#ifndef __SULJSONARRAY_H__
#define __SULJSONARRAY_H__

/*
	[...]

	can contain values or objects
*/

#include "SulJson.h"
#include "SulTypes.h"
#include "SulExport.h"

class SUL_EXPORT CSulJsonArray : public CSulJson
{
private:
	typedef std::vector<CSulJson*>	VEC_JSON;

public:
	CSulJsonArray();
	CSulJsonArray( CSulJson* pParent );

	void add( CSulJson* p );

	CSulJson* get( sigma::uint32 index );

	sigma::uint32 getCount();

	CSulJsonArray* asArray() { return this; } 

private:
	VEC_JSON	m_vecJson;
};

#endif // __SULJSONARRAY_H__