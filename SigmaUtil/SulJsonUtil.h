// SulJsonUtil.h

#ifndef __SULJSONUTIL_H__
#define __SULJSONUTIL_H__

#include "SulString.h"
#include "SulJson.h"
#include "SulExport.h"

class SUL_EXPORT CSulJsonUtil
{
public:
	CSulJson*	create( class CSulParser* parser, CSulJson* curJson=0 );

	// converts a json string to CSulJson objects
	CSulJson*	create( const CSulString& json );
};

#endif // __SULJSONUTIL_H__