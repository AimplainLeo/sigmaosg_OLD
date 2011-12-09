// SulJsonObject.h

#ifndef __SULJSONOBJECT_H__
#define __SULJSONOBJECT_H__

/*
	{...}

	can contain a list of values or arrays
*/

#include "SulJson.h"
#include "SulString.h"
#include "SulExport.h"

class SUL_EXPORT CSulJsonObject : public CSulJson
{
private:
	typedef std::map<CSulString, CSulJson*>		MAP_KEYVAL;
public:
	CSulJsonObject();
	CSulJsonObject( CSulJson* pParent );

	void add( const CSulString& key, CSulJson* p );

	CSulJson* get( const CSulString& key );

	class CSulJsonObject* asObject() { return this; } 

private:
	VECTOR_JSON	m_listJson;

	MAP_KEYVAL	m_mapKeyVal;
};

#endif // __SULJSONOBJECT_H__