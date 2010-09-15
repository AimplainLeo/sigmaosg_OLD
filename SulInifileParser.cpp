#include "StdAfx.h"
#include "SulInifileParser.h"
#include "SulParser.h"
#include "SulString.h"
#include <iostream>

/*!
\class CIniFileParser
\brief Provides easy access to data contained in ini files.
\author Michael Bach Jensen

This class supports sections, keys and values. Construct it with the path to an ini file
and retrieve values with GetValue
*/

CSulIniFileParser::CSulIniFileParser()
{
}


/*!
\brief Constructor that loads and parses the given ini file
*/
CSulIniFileParser::CSulIniFileParser(const std::string& iniFilePath)
{
	SetFilePath(iniFilePath);
}


/*!
\brief Loads and parses the given ini file
*/
bool CSulIniFileParser::SetFilePath( const std::string& iniFilePath )
{
	m_sectionMap.clear();

	// Set the default section to "General".
	// Key-value pairs that are not placed under a section in the ini file end up 
	// under the General section
	m_sCurrentSection = "General";
	m_sectionMap.insert(SectionMap::value_type(m_sCurrentSection, KeyValueMap()));

	osg::ref_ptr<CSulParser> parser = CreateParser();
	if(!parser->Init(iniFilePath))
	{
		return false;
	}

	while ( char* p = parser->PeekToken() )
	{
		if ( *p == '[' )
		{
			ParseSection(parser.get());
		}
		else
		{
			ParseKeyValuePair(parser.get());
		}
	}

	return true;
}


/*!
\brief Looks up \a key under \a section in the given ini file and puts the value in \a val
\return false if the key/section could not be found.

The default \a section is used to look up keys that are not under a specific section (or under the [General] section!).
In addition, keys and the section are case insensitive.
*/
bool CSulIniFileParser::GetString( std::string& val, const std::string& key, const std::string& defaultValue, const std::string& section /*= "General"*/ )
{
	std::string sectionLower = GetLowerCaseVersion(section);
	std::string keyLower = GetLowerCaseVersion(key);

	SectionMap::const_iterator i = m_sectionMap.find(sectionLower);
	if(i == m_sectionMap.end())
	{
		val = defaultValue;
		return false;
	}

	const KeyValueMap& keyValueMap = i->second;
	KeyValueMap::const_iterator j = keyValueMap.find(keyLower);
	if ( j == keyValueMap.end() )
	{
		val = defaultValue;
		return false;;
	}

	val = j->second;
	return true;
}


/*!
\brief As GetString, but for retrieving an integer value
\sa GetString
*/
bool CSulIniFileParser::GetInteger( int& val, const std::string& key, int defaultValue, const std::string& section /*= "General"*/ )
{
	std::string s;
	//@TKG we need both the default and section value when calling string since the method have a default value
	if(!GetString(s, key, "" ,section))
	{
		val = defaultValue;
		return false;
	}
	
	val = atoi(s.c_str());
	return true;
}


/*!
\brief As GetString, but for retrieving a double value
\sa GetString
*/
bool CSulIniFileParser::GetDouble( double& val, const std::string& key, double defaultValue /*= 0.0*/, const std::string& section /*= "General"*/ )
{
	std::string s;
	//@TKG we need both the default and section value when calling string since the method have a default value
	if(!GetString(s, key, "" ,section))
	{
		val = defaultValue;
		return false;
	}

	val = atof(s.c_str());
	return true;
}


/*!
\brief As GetString, but for retrieving a boolean value (which can be written as both an integer and the words true/false)
\sa GetString
*/
bool CSulIniFileParser::GetBoolean( bool& val, const std::string& key, bool defaultValue, const std::string& section /*= "General"*/ )
{
	std::string s;
	if(!GetString(s, key, "", section))
	{
		val = defaultValue;
		return false;
	}

	s = GetLowerCaseVersion(s);
	if(s == "t" || s == "true" || atoi(s.c_str()) > 0)
	{
		val = true;
	}
	else
	{
		val = false;
	}

	return true;
}


CSulParser* CSulIniFileParser::CreateParser()
{
	CSulParser* parser = new CSulParser;
	parser->RemoveCharToken(';');
	parser->RemoveCharToken('=');
	parser->AddCommentEOL(";");
	parser->AddSentenceCharStart('=');
	parser->AddSentenceCharEnd('\n');
	parser->AddSentenceCharEnd('\r');
	return parser;
}


void CSulIniFileParser::AddKeyValuePair( const CSulString& key, const CSulString& val, const CSulString& section )
{
	SectionMap::iterator sectionMapIterator = m_sectionMap.find(section);
	assert(sectionMapIterator != m_sectionMap.end());

	KeyValueMap& keyValueMap = sectionMapIterator->second;
	if(keyValueMap.find(key) != keyValueMap.end())
	{
		//LOG_ERROR("Ini file contains multiple sections with the same name! ");
	}
	else
	{
		keyValueMap.insert(KeyValueMap::value_type(key, val));
	}
}


void CSulIniFileParser::ParseSection( CSulParser* parser )
{
	parser->GetToken(); // eat open bracket
	CSulString sectionName = parser->GetStringToken();
	sectionName.MakeLower();
	parser->GetToken(); // eat closing bracket

	// add new section to the section map if one does not already exist
	if(m_sectionMap.find(sectionName) == m_sectionMap.end())
	{
		m_sectionMap.insert(SectionMap::value_type(sectionName, KeyValueMap()));
	}

	m_sCurrentSection = sectionName;
}


void CSulIniFileParser::ParseKeyValuePair( CSulParser* parser )
{
	CSulString key = parser->GetStringToken();
	CSulString val = parser->GetStringToken();
	val = val.substr(0, val.find(';')); // strip possible comments from the value
	val.Trim(); // trim leading and trailing white space
	key.MakeLower();
	AddKeyValuePair(key, val, m_sCurrentSection);
}


void CSulIniFileParser::DumpContentsToConsole()
{
	for( SectionMap::const_iterator it = m_sectionMap.begin(); it != m_sectionMap.end(); ++it )
	{
		const KeyValueMap& keyValueMap = it->second;
		std::cout << "Section=" << it->first << std::endl;
		for( KeyValueMap::const_iterator it = keyValueMap.begin(); it != keyValueMap.end(); ++it )
		{
			std::cout << "Key=" << it->first << ", value=" << it->second << std::endl;
		}
	}
}


std::string CSulIniFileParser::GetLowerCaseVersion( const std::string &s )
{
	std::string sl = s;
	std::transform( sl.begin(), sl.end(), sl.begin(), tolower );
	return sl;
}