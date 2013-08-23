// networkserver.cpp

#include "stdafx.h"
#include "networkserver.h"
#include <iostream>
#include <WinSock2.h>

CNetworkServer::CNetworkServer( const CSulString& hostName, sigma::uint32 port )
{
	m_debug				= false;
	m_socketServer		= 0;
	m_hostName			= hostName;
	m_port				= port;
	init();
}

void CNetworkServer::init()
{
	m_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if ( m_socketServer==SOCKET_ERROR )
	{
		std::cout << "Server: Socket Failed to initialize!\n";
		return;
	}

	struct sockaddr_in server; // New! Our network socket structure
	server.sin_family = AF_INET; // Always have ti as this for internet
	server.sin_port = htons(m_port); // Dont forget the byte ordering

	hostent* localHost;
	char* localIP;
	localHost = gethostbyname(m_hostName.c_str());
	localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

	server.sin_addr.s_addr = inet_addr(localIP);

	// Attempt to bind the socket
	int error;
	error = bind( m_socketServer, (sockaddr*)&server, sizeof(server) ); 
	if (error == SOCKET_ERROR)
	{
		std::cout << "Server: bind failed\n";
		return;
	}

	// Listen for connections
	error = listen( m_socketServer, 5 ); 
	if (error == SOCKET_ERROR)
	{
		std::cout << "Server: listen failed\n"; 
		return;
	}
}

void CNetworkServer::debug()
{
	m_debug = true;
}

bool CNetworkServer::isDebug()
{
	return m_debug;
}

void CNetworkServer::sendStringToAll( const CSulString& s )
{
	VEC_SOCKETS::iterator i = m_vecSockets.begin();
	VEC_SOCKETS::iterator ie = m_vecSockets.end();

	while ( i!=ie )
	{
		int bytesSent = send( *i, s.c_str(), s.size(), 0);
		if ( bytesSent == SOCKET_ERROR)
			std::cout << "Server : send() error " << WSAGetLastError() << std::endl;

		++i;
	}
}

void CNetworkServer::run()
{
	if ( m_socketServer==0 )
		return;

	std::cout << "Server: Waiting for a client to connect ...\n"; // Just to keep us up to date - ish
	
	// Accepts the client (it is prepared to handle more than one socket.. but the code only accepts one socket right now)
	int socketAccepted = accept(m_socketServer, 0, 0); 
	if ( socketAccepted==SOCKET_ERROR )
	{
		std::cout << "Server: will not accept\n"; 
		return;
	}
	m_vecSockets.push_back( socketAccepted );

	std::cout << "Server: Client Connected!\n";
	char recvbuf[200];
	bool connected = true;
	while ( connected )
	{
		int bytesRecv = SOCKET_ERROR;
		bytesRecv = recv( socketAccepted, recvbuf, 199, 0 );
		if ( bytesRecv!=SOCKET_ERROR )
		{
			recvbuf[bytesRecv] = 0; // we zero terminate

			if ( isDebug() )
			{
				std::cout << "Server: Received (" << bytesRecv << "): " << recvbuf << std::endl;
				sendStringToAll( "bounce back: "+CSulString(recvbuf) ); // we bounce the message back
			}

			CSulStringList s;
			s.explode( recvbuf, ';' );
			processCommands( s );
		}
		else
		{
			printf("Server: recv() error %ld.\n", WSAGetLastError());
		}
	}
}

void CNetworkServer::processCommands( const CSulStringList& list )
{
	// get variables to work with processing the list
	const CSulStringList::VECTOR_STRING& vecList = list.getList();
	CSulStringList::VECTOR_STRING::const_iterator i = vecList.cbegin();
	CSulStringList::VECTOR_STRING::const_iterator ie = vecList.cend();

	while ( i!=ie )
	{
		CSulStringList s;
		s.explode( i->c_str(), ':' );
		command( s );
		i++;
	}
}

void CNetworkServer::command( const CSulStringList& list )
{
}
