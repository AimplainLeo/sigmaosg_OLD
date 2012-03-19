// SulJson.h

#ifndef __SULJSON_H__
#define __SULJSON_H__

#include <osg/referenced>

class CSulJson : public osg::Referenced
{
public:
	CSulJson();
	CSulJson( CSulJson* pParent );

	virtual CSulJson* getParent() const;

	virtual void add( CSulJson* p ) {}

	virtual class CSulJsonArray* asArray() { return 0; } 
	virtual class CSulJsonObject* asObject() { return 0; } 
	virtual class CSulJsonString* asString() { return 0; }

private:
	CSulJson* m_pParent;
};

typedef std::vector<CSulJson> VECTOR_JSON;

#endif // __SULJSON_H__