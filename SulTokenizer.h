// SulTokenizer.h

#ifndef __SULTOKENIZER_H__
#define __SULTOKENIZER_H__

#include "SulTypes.h"
#include "SulString.h"

class CSulTokenizer : public osg::Referenced
{
private:
	typedef std::vector<CSulString>	VECTOR_STRING;

public:
						CSulTokenizer();

	bool				Parse( sigma::uint8* pData, sigma::uint32 iSize );

	const CSulString&	GetToken( sigma::uint32 iIndex );

	// character tokens
	void				AddCharAsToken( sigma::uint8 c );
	void				RemoveCharAsToken( sigma::uint8 c );
	bool				IsCharToken( sigma::uint8 c ) { return m_cToken[c]?true:false; }

	// character delimiters
	void				AddCharAsDelimiter( sigma::uint8 c );
	void				RemoveCharAsDelimiter( sigma::uint8 c );
	bool				IsCharDelimiter( sigma::uint8 c ) { return m_cDelimiter[c]?true:false; }

	// character sentence start
	void				AddCharAsSentenceStart( sigma::uint8 c );
	void				RemoveCharAsSentenceStart( sigma::uint8 c );
	bool				IsCharSentenceStart( sigma::uint8 c ) { return m_cSentenceStart[c]?true:false; }

	// character sentence end
	void				AddCharAsSentenceEnd( sigma::uint8 c );
	void				RemoveCharAsSentenceEnd( sigma::uint8 c );
	bool				IsCharSentenceEnd( sigma::uint8 c ) { return m_cSentenceEnd[c]?true:false; }

private:
	VECTOR_STRING	m_vecToken;
	sigma::int8		m_cToken[256];
	sigma::int8		m_cDelimiter[256];
	sigma::int8		m_cSentenceStart[256];
	sigma::int8		m_cSentenceEnd[256];
};

#endif // __SULTOKENIZER_H__