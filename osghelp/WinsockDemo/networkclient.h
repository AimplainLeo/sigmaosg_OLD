// networkclient.h

#ifndef __NETWORKCLIENT_H__
#define __NETWORKCLIENT_H__

#include <SigmaUtil/SulString.h>
#include <SigmaUtil/SulStringList.h>
#include <OpenThreads/thread>

class CNetworkClient : public OpenThreads::Thread
{
public:
	CNetworkClient( const CSulString& hostName="", sigma::uint32 port=9000 );

	void CNetworkClient::sendString( const CSulString& s );
	void run();

private:
	void init();

private:
	int				m_socketClient;
	CSulString		m_hostName;
	sigma::uint32	m_port;

};

#endif // __NETWORKCLIENT_H__