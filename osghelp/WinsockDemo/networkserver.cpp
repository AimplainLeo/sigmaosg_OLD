// networkserver.cpp

#include "stdafx.h"
#include "networkserver.h"
#include <iostream>
#include <WinSock2.h>

CNetworkServer::CNetworkServer( const CSulString& hostName, sigma::uint32 port )
{
	m_socketServer	= 0;
	m_hostName		= hostName;
	m_port			= port;
	m_debug			= false;
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

	int error;
	error = bind( m_socketServer, (sockaddr*)&server, sizeof(server) ); // Attempt to bind the socket
	if (error == SOCKET_ERROR)
	{
		std::cout << "Server: Ah ****... Somethings wrong with binding\n";
		return;
	}

	error = listen( m_socketServer, 5 ); // Listen for connections
	if (error == SOCKET_ERROR)
	{
		std::cout << "Server: Too deaf to listen...\n"; // Did we error?!?!
		return;
	}
}

void CNetworkServer::debug()
{
	m_debug = true;
}

void CNetworkServer::run()
{
	if ( m_socketServer==0 )
		return;

	std::cout << "Server: Waiting for a client to connect ...\n"; // Just to keep us up to date - ish
	int clientSocket; // Used for the client

	clientSocket = accept(m_socketServer, 0, 0); // Accepts the client
	if (clientSocket == SOCKET_ERROR)
	{
		std::cout << "I DUN WANNA!\n"; // did we Fu** up again?
		return;
	}

	std::cout << "Server: Client Connected!\n";
	char recvbuf[200];
	bool connected = true;
	while ( connected )
	{
		int bytesRecv = SOCKET_ERROR;
		bytesRecv = recv( clientSocket, recvbuf, 199, 0 );
		if ( bytesRecv!=SOCKET_ERROR )
		{
			recvbuf[bytesRecv] = 0; // we zero terminate

			if ( m_debug )
				std::cout << "Received (" << bytesRecv << "): " << recvbuf << std::endl;

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
