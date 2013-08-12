// Grid.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulStringList.h>
#include <osgViewer/Viewer>

void processCommands( const CSulStringList& list );
void zoom( float v );

osg::ref_ptr<osgViewer::Viewer> viewer;

int mySocketServer = 0;
int mySocketClient = 0;

osg::Node* createScene()
{
	// using the sigma utility library to create the grid
	CSulGeomGrid* pGeomGrid = new CSulGeomGrid;
	pGeomGrid->Create( osg::Vec3(0,0,0), 4, 4, 1, 1, 5, 5 );

	return pGeomGrid;
}


#include <iostream>
#include <WinSock2.h>

class CServerThread : public OpenThreads::Thread
{
public:
	CServerThread()
	{
	}

	void run()
	{
		if ( mySocketServer==0 )
			return;

		std::cout << "Server: Waiting for a client to connect ...\n"; // Just to keep us up to date - ish
		int clientSocket; // Used for the client

		clientSocket = accept(mySocketServer, 0, 0); // Accepts the client
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
				//std::cout << "Received (" << bytesRecv << "): " << recvbuf << std::endl;

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

};

class CClientThread : public OpenThreads::Thread
{
public:
	CClientThread()
	{
	}

	void run()
	{
		if ( mySocketClient==0 )
			return;

		struct sockaddr_in server; // New! Our network socket structure
		server.sin_family = AF_INET; // Always have ti as this for internet
		server.sin_port = htons(9000); // Dont forget the byte ordering

		hostent* localHost;
		char* localIP;
		localHost = gethostbyname("");
		localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

		server.sin_addr.s_addr = inet_addr(localIP);

		std::cout << "Client: connecting...\n";
		int error;
		error = connect(mySocketClient, (sockaddr*)&server, sizeof(server)); // Lets attempt to connect now
		if ( error==SOCKET_ERROR )
		{
			std::cout << "Client: Damn server wont let me connect!\n";
			return;
		}

		std::cout << "Client: connected to the server!\n";

		while (1)
		{
			char* sendbuf = "quick brown fox";
			int bytesSent = send(mySocketClient, sendbuf, strlen(sendbuf), 0);
			if ( bytesSent == SOCKET_ERROR)
				printf("Client: send() error %ld.\n", WSAGetLastError());

			Sleep(1000);
		}
	}

};

void setupNetwork()
{
	WSAData wsaData;

	int error;

	error = WSAStartup(MAKEWORD(2, 2), &wsaData); // Make a call to start up winsock, MAKEWORD simply tells the version we are going to us. Version 2.2 for us
    if (error == SOCKET_ERROR)  // Almost EVERY Winsock function will reutrn a number on error, so we should always check for errors
    {
        std::cout << "Server: We Errored Starting Winsock!!!\n";
        return;
    }
}

void testServer()
{
	mySocketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocketServer == SOCKET_ERROR)
    {
		std::cout << "Server: Socket Failed to initialize!\n";
		return;
    }

	struct sockaddr_in server; // New! Our network socket structure
    server.sin_family = AF_INET; // Always have ti as this for internet
    server.sin_port = htons(9000); // Dont forget the byte ordering

	hostent* localHost;
	char* localIP;
	localHost = gethostbyname("");
	localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

	server.sin_addr.s_addr = inet_addr(localIP);

	int error;
    error = bind(mySocketServer, (sockaddr*)&server, sizeof(server)); // Attempt to bind the socket
    if (error == SOCKET_ERROR)
    {
		std::cout << "Server: Ah ****... Somethings wrong with binding\n";
		return;
    }

	error = listen(mySocketServer, 5); // Listen for connections
	if (error == SOCKET_ERROR)
    {
		std::cout << "Server: Too deaf to listen...\n"; // Did we error?!?!
		return;
    }
	
	CServerThread* t = new CServerThread();
	t->startThread();
}

void testClient()
{
	mySocketClient = socket(AF_INET, SOCK_STREAM, 0);

    if (mySocketClient == SOCKET_ERROR)
    {
        std::cout << "Client: Socket Door is stuck.. Wont open.. -.-;\n";
		return;
    }

	CClientThread* t = new CClientThread();
	t->startThread();
}

void processCommands( const CSulStringList& list )
{
	// get variables to work with processing the list
	const CSulStringList::VECTOR_STRING& vecList = list.getList();
	CSulStringList::VECTOR_STRING::const_iterator i = vecList.cbegin();
	CSulStringList::VECTOR_STRING::const_iterator ie = vecList.cend();

	while ( i!=ie )
	{
		CSulStringList s;
		s.explode( i->c_str(), ':' );

		CSulString command = s.getString(0);

		if ( command=="item_vector_zoom" )
		{
			sigma::int32 value = 0;
			value = s.getString(1).asInt32();
			std::cout << "COMMAND: " << command << " VALUE: " << value << std::endl;
			zoom( ((float)value/100.0f) );
		}

		i++;
	}
}

void zoom( float v )
{
	osg::Camera* cam = viewer->getCamera();
	osg::Matrix projMatrix = cam->getProjectionMatrix();
	double fovy, aspectRatio, zNear, zFar;
	projMatrix.getPerspective( fovy, aspectRatio, zNear, zFar );
	fovy = 29.0 - 20.0*v;
	projMatrix.makePerspective( fovy, aspectRatio, zNear, zFar );
	cam->setProjectionMatrix( projMatrix );
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
	viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

	// set the scene-graph data the viewer will render
	osg::Node* pScene = createScene();
  
	viewer->setSceneData( pScene );

	setupNetwork();
	testServer();
//	testClient();

	/*
	osg::Camera* cam = viewer->getCamera();
	osg::Matrix projMatrix = cam->getProjectionMatrix();
	double fovy, aspectRatio, zNear, zFar;
	projMatrix.getPerspective( fovy, aspectRatio, zNear, zFar );
	std::cout << fovy << std::endl;
	*/

	// execute main loop
	return viewer->run();
}
