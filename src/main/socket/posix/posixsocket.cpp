//-----------------------------------------------------------------------------
#include <cstring>
#include <iostream>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

#include "posixsocket.h"


using namespace std;

shared_ptr<Socket> Socket::create() {
	return make_shared<PosixSocket>();
}

//-----------------------------------------------------------------------------
PosixSocket::PosixSocket() {
}

//-----------------------------------------------------------------------------
PosixSocket::~PosixSocket() {
	deinit();
}
//-----------------------------------------------------------------------------
bool PosixSocket::Connect(std::string hostIp,int hostPort){
	// Usamos connect cuando tenemos que conectarnos a un server

	// Obtenemos host
	struct hostent *he = gethostbyname(hostIp.c_str());
	if (!he) {
		return false;
	}

	// Obtenemos socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return false;
	}

	// Cargamos datos de la conexión a realizar
	memset(&sockaddr, '\0', sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(hostPort);
	memcpy(&sockaddr.sin_addr, he->h_addr, he->h_length);

	// Conectamos
	if(connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr)) == -1) {
		return false;
	}

	Activate();

	return true;
}
//-----------------------------------------------------------------------------
bool PosixSocket::Listen(unsigned int port, int maxConnections) {
	cerr << "listen()" << endl;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return false;
	}

	Activate();

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);

	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
		return false;
	}

	// Comenzamos la escucha
	if(listen(sockfd, maxConnections) < 0) {
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
shared_ptr<Socket> PosixSocket::Accept() {
	unsigned sin_size = sizeof(struct sockaddr_in);
	//Revisar
	auto socket_client = make_shared<PosixSocket>();

	int sockfd_client = accept(sockfd, (struct sockaddr *)&(socket_client->sockaddr), &sin_size);

	socket_client->sockfd = sockfd_client;
	// Corroboramos si no se cerró el socket
	if(status != 1) {
		return nullptr;
	}

	socket_client->Activate();

	return socket_client;

}
//-----------------------------------------------------------------------------
ssize_t PosixSocket::Send(const void* data, size_t dataLenght) {
	cerr << "send()" << endl;
	// Cantidad de bytes que han sido enviados
	size_t total_bytes = 0;

	for(ssize_t n = 0; total_bytes < dataLenght; total_bytes += n) {
		// Realizamos envío de bytes
		n = send(sockfd, (char *) data + total_bytes, dataLenght - total_bytes, MSG_NOSIGNAL);

		if(n < 0) {
			deinit();
			return n;
		}
	}

	return total_bytes;

}
//-----------------------------------------------------------------------------
ssize_t PosixSocket::Recv(void* data, size_t dataLenght) {
	memset(data, '\0', dataLenght);
	return recv(sockfd, data, dataLenght, 0);
}
//-----------------------------------------------------------------------------
bool PosixSocket::IsActive() {
	return status;
}
//-----------------------------------------------------------------------------
void PosixSocket::deinit() {
	this->status = false;
	close(sockfd);
}
//-----------------------------------------------------------------------------
void PosixSocket::Activate() {
	this->status = true;
}
//-----------------------------------------------------------------------------
