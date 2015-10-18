//-----------------------------------------------------------------------------
#include "posixsocket.h"
#include <stdlib.h>
//-----------------------------------------------------------------------------
PosixSocket::PosixSocket(unsigned int uiPort) {
	this->port = uiPort;
	this->sockfd = 0;
}
//-----------------------------------------------------------------------------
PosixSocket::PosixSocket(){

}
//-----------------------------------------------------------------------------
PosixSocket::~PosixSocket() {
	// TODO Auto-generated destructor stub
}
//-----------------------------------------------------------------------------
void PosixSocket::Connect(std::string hostIP,int hostPort){


}
//-----------------------------------------------------------------------------
bool PosixSocket::Listen(int maxConnections)
{
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);

	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(	bind(sockfd,
			(struct sockaddr *)&sockaddr,
			sizeof(sockaddr)) < 0)
	{
		return -1;
	}

	// Comenzamos la escucha
	return listen(sockfd, maxConnections);
}
//-----------------------------------------------------------------------------
Socket* PosixSocket::Accept()
{
	unsigned sin_size = sizeof(struct sockaddr_in);
	//Revisar
	PosixSocket* socket_client = new PosixSocket();

	int sockfd_client = accept(sockfd, (struct sockaddr *)&socket_client->sockaddr, &sin_size);

	socket_client->sockfd = sockfd_client;
	// Corroboramos si no se cerró el socket
	if(status != 1) return 0;

	return (Socket*)socket_client;

}
//-----------------------------------------------------------------------------
int PosixSocket::Send(const void* data, int dataLenght)
{

	// Cantidad de bytes que han sido enviados
	int total_bytes = 0;
	// Cantidad de bytes que faltan enviar
	int residuary_bytes = dataLenght;
	// Variable auxiliar
	int n;

	while(residuary_bytes > 0)
	{
		// Realizamos envío de bytes
		n = send(sockfd, (char *) data + total_bytes, residuary_bytes, 0);

		if(n == -1)	return -1;

		total_bytes += n;
		residuary_bytes -= n;
	}

	return 0;

}
//-----------------------------------------------------------------------------
int PosixSocket::Recv(const void* data, int dataLenght)
{
	//REVISAR
	//memset(data, '\0', dataLenght);
	// Recibimos datos en buffer
	return recv(sockfd, &data, dataLenght, 0);
}
//-----------------------------------------------------------------------------
bool PosixSocket::IsActive()
{
	return status;
}
//-----------------------------------------------------------------------------
