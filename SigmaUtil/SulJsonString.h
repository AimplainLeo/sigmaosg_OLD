// SulJsonString.h

#ifndef __SULJSONSTRING_H__
#define __SULJSONSTRING_H__

#include "SulJson.h"
#include "SulString.h"
#include "SulExport.h"

class SUL_EXPORT CSulJsonString : public CSulJson
{
public:
	CSulJsonString( const CSulString& s );

	class CSulJsonString* asString() { return this; }

	CSulString get() const;

private:
	CSulString m_s;
};

#endif // __SULJSONSTRING_H__