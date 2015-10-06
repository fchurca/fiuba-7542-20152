//-----------------------------------------------------------------------------
#include "socket.h"
//-----------------------------------------------------------------------------
Socket::Socket(unsigned int port)
:status(false)
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
	}

	addr.sin_family = AF_INET;      // Address family
	addr.sin_port = htons (port);   // Assign port to this socket
	addr.sin_addr.s_addr = htonl (INADDR_ANY);
	sockfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket
}
//-----------------------------------------------------------------------------
Socket::~Socket() {

	if (sockfd)
		closesocket(sockfd);
	WSACleanup(); //Clean up Winsock
}
//-----------------------------------------------------------------------------

