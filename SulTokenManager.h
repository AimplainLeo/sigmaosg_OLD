// SulTokenManager.h

#ifndef __SULTOKENMANAGER_H__
#define __SULTOKENMANAGER_H__

#include "SulString.h"
#include "SulTokenizer.h"
#include <osg/Referenced>

class CSulTokenManager : public osg::Referenced
{
private:
	typedef std::vector<CSulString>	VECTOR_STRING;

public:
									CSulTokenManager();

	bool							Parse( sigma::uint8* pData, sigma::uint32 iSize );

	// gets the current token and advances the index to the next token
	const char*						GetToken();

private:
	osg::ref_ptr<CSulTokenizer>		m_rTokenizer;
	VECTOR_STRING	m_vecToken;
	sigma::int32	m_iIndex;
};

#endif // __SULTOKENMANAGER_H__