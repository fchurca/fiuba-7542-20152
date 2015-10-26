//-----------------------------------------------------------------------------
#include "posixsocket.h"
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>

#include <iostream>
using namespace std;
//-----------------------------------------------------------------------------
PosixSocket::PosixSocket() {

}

//-----------------------------------------------------------------------------
PosixSocket::~PosixSocket() {
	//if(close(sockfd) == -1)
	//		std::cerr << "ERROR: No se ha podido cerrar el socket." << std::endl
}
//-----------------------------------------------------------------------------
bool PosixSocket::Connect(std::string hostIp,int hostPort){
	cerr << "connect()" << endl;
	// Usamos connect cuando tenemos que conectarnos a un server

	// Obtenemos host
	struct hostent *he = gethostbyname(hostIp.c_str());

	// Cargamos datos de la conexión a realizar
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(hostPort);
	// destinoDir.sin_addr.s_addr = inet_addr(ipDestino.c_str());
	sockaddr.sin_addr = *((struct in_addr *)he->h_addr);

	//Ver si es necesario
	//memset(&(sockaddr.sin_zero), '\0', sizeof(sockaddr.sin_zero));

	// Conectamos
	if(connect(this->sockfd, (struct sockaddr *)&sockaddr,
		sizeof(struct sockaddr)) == -1)
		throw "ERROR: No se pudo llevar a cabo la conexión.";


	return true;
}
//-----------------------------------------------------------------------------
bool PosixSocket::Listen(unsigned int port, int maxConnections)
{
	cerr << "listen()" << endl;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return false;

	status =  true;

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);

	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(	bind(sockfd,
			(struct sockaddr *)&sockaddr,
			sizeof(sockaddr)) < 0)
	{
		return false;
	}

	// Comenzamos la escucha
	if ( listen(sockfd, maxConnections) < 0)
	{
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
Socket* PosixSocket::Accept()
{
	cerr << "accept()" << endl;
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
	cerr << "send()" << endl;
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
	cerr << "receive()" << endl;
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
void PosixSocket::deinit()
{
	this->status = false;
}
//-----------------------------------------------------------------------------
void PosixSocket::Activate()
{
	this->status = true;
}
//-----------------------------------------------------------------------------
