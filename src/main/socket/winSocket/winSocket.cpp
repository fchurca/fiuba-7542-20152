//-----------------------------------------------------------------------------
#include "winSocket.h"
//-----------------------------------------------------------------------------
WinSocket::WinSocket(unsigned int uiPort)
{
	int error = WSAStartup (0x0202, &wsaData);   // Fill in WSA info
	SOCKADDR_IN addr; // The address structure for a TCP socket

	if (error)
		exit(1);

	//Did we get the right Winsock version?
	if (wsaData.wVersion != 0x0202)
	{
		WSACleanup(); //Clean up Winsock
		exit(1);
		// reemplazar por log + handle
	}

	addr.sin_family = AF_INET;      // Address family
	addr.sin_port = htons (port);   // Assign port to this socket
	addr.sin_addr.s_addr = htonl (INADDR_ANY);
	sockfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket

	if (sockfd == INVALID_SOCKET)
	{
		exit(1); //Don't continue if we couldn't create a //socket!!
		// reemplazar por log + handle
	}
}
//-----------------------------------------------------------------------------
WinSocket::~WinSocket() {

	if (sockfd)
		closesocket(sockfd);
	WSACleanup(); //Clean up Winsock
}
//-----------------------------------------------------------------------------
bool Socket::Listen(int maxConnections){

	if (bind(sockfd, (LPSOCKADDR)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
	   //We couldn't bind (this will happen if you try to bind to the same
	   //socket more than once)
		return false;
	}
	listen(sockfd, maxConnections);
	return true;
}
//-----------------------------------------------------------------------------
Socket* WinSocket::Accept(){



	return true;

}
//-----------------------------------------------------------------------------
