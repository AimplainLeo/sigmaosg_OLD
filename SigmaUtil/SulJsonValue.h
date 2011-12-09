// SulJsonValue.h

#ifndef __SULJSONVALUE_H__
#define __SULJSONVALUE_H__

/*
	key : value
*/

class CSulJsonValue : public CSulJson
{
public:

private:
	CSulString	m_key;
	CSulString	m_value;
};

#endif // __SULJSONVALUE_H__