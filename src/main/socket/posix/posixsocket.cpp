//-----------------------------------------------------------------------------
#include <cstring>

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
	memset(&sockaddr.sin_zero, '\0', sizeof(sockaddr.sin_zero));
	sockaddr.sin_family = AF_INET;
}

//-----------------------------------------------------------------------------
PosixSocket::~PosixSocket() {
	deinit();
}
//-----------------------------------------------------------------------------
bool PosixSocket::Connect(std::string hostIp,size_t hostPort){
	// Usamos connect cuando tenemos que conectarnos a un server

	// Obtenemos host
	struct hostent *he = gethostbyname(hostIp.c_str());
	if (!he) {
		return false;
	}

	// Obtenemos socket
	auto s = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0) {
		return false;
	}
	sockfd = s;

	// Cargamos datos de la conexión a realizar
	sockaddr.sin_port = htons(hostPort);
	memcpy(&sockaddr.sin_addr, he->h_addr, he->h_length);

	// Conectamos
	if(connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
		return false;
	}

	Activate();

	return true;
}
//-----------------------------------------------------------------------------
bool PosixSocket::Listen(size_t port, size_t maxConnections) {
	auto s = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0) {
		return false;
	}

	sockfd = s;

	Activate();

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
	socklen_t sin_size = sizeof(struct sockaddr_in);

	auto socket_client = make_shared<PosixSocket>();

	auto sockfd_client = accept(sockfd, (struct sockaddr *)&(socket_client->sockaddr), &sin_size);
	// Corroboramos si no se cerró el socket
	if(sockfd_client < 0) {
		return nullptr;
	}

	socket_client->sockfd = sockfd_client;
	socket_client->Activate();

	return socket_client;

}
//-----------------------------------------------------------------------------
long PosixSocket::Send(const void* data, size_t dataLength) {
	// Cantidad de bytes que han sido enviados
	size_t total_bytes = 0;

	for(long n = 0; total_bytes < dataLength; total_bytes += n) {
		// Realizamos envío de bytes
		n = send(sockfd, (char *) data + total_bytes, dataLength - total_bytes, MSG_NOSIGNAL);

		if(n < 0) {
			deinit();
			return n;
		}
	}

	return total_bytes;

}
//-----------------------------------------------------------------------------
long PosixSocket::Recv(void* data, size_t dataLength) {
	memset(data, '\0', dataLength);
	return recv(sockfd, data, dataLength, 0);
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
