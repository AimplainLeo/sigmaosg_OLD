#ifndef __InifileParser_H__
#define __InifileParser_H__

#include "SulExport.h"
class CSulString;
class CSulParser;

class SUL_EXPORT CSulIniFileParser
{
public:
	CSulIniFileParser(const std::string& iniFilePath);
	CSulIniFileParser();

	bool SetFilePath(const std::string& iniFilePath);

	bool GetString(std::string& val, const std::string& key, const std::string& defaultValue = "", const std::string& section = "General");
	bool GetInteger(int& val, const std::string& key, int defaultValue = 0, const std::string& section = "General");
	bool GetDouble(double& val, const std::string& key, double defaultValue = 0.0, const std::string& section = "General");
	bool GetBoolean(bool& val, const std::string& key, bool defaultValue = false, const std::string& section = "General");

private:
	CSulParser* CreateParser();
	void ParseSection( CSulParser* parser );
	void ParseKeyValuePair( CSulParser* parser );
	void AddKeyValuePair( const CSulString& key, const CSulString& val, const CSulString& section );
	void DumpContentsToConsole();
	std::string GetLowerCaseVersion( const std::string &s );

private:
	typedef std::map<std::string, std::string> KeyValueMap;
	typedef std::map<std::string, KeyValueMap> SectionMap;

	SectionMap m_sectionMap;
	std::string m_sCurrentSection;
};

#endif // __InifileParser_H__
