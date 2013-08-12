// networkserver.h

#ifndef __NETWORKSERVER_H__
#define __NETWORKSERVER_H__

#include <SigmaUtil/SulString.h>
#include <SigmaUtil/SulStringList.h>
#include <OpenThreads/thread>

class CNetworkServer : public OpenThreads::Thread
{
public:
	CNetworkServer( const CSulString& hostName="", sigma::uint32 port=9000 );

	void debug();
	void run();

	// FIXME: need some posix stuff here to prevent thread collisions
	virtual void command( const CSulStringList& list );

private:
	void init();
	void processCommands( const CSulStringList& list );

private:
	bool			m_debug;
	int				m_socketServer;
	CSulString		m_hostName;
	sigma::uint32	m_port;
};

#endif // __NETWORKSERVER_H__