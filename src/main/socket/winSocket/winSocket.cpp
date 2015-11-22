//-----------------------------------------------------------------------------
#include "winSocket.h"
//-----------------------------------------------------------------------------
using namespace std;

shared_ptr<Socket> Socket::create() {
	//return make_shared<WinSocket>();
	return nullptr;
}

WinSocket::WinSocket() {

}

//-----------------------------------------------------------------------------
WinSocket::~WinSocket() {
	//if(close(sockfd) == -1)
	//		std::cerr << "ERROR: No se ha podido cerrar el socket." << std::endl
}

bool WinSocket::Connect(std::string hostIp, int hostPort) {
	//cerr << "connect()" << endl;
	// Usamos connect cuando tenemos que conectarnos a un server

	// Obtenemos host
	//struct hostent *he = gethostbyname(hostIp.c_str());
	//if (!he) {
	//	return false;
	//}
	//
	//// Obtenemos socket
	//if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	//	return false;
	//}
	//
	//// Cargamos datos de la conexión a realizar
	//memset(&sockaddr, '\0', sizeof(sockaddr));
	//sockaddr.sin_family = AF_INET;
	//sockaddr.sin_port = htons(hostPort);
	//memcpy(&sockaddr.sin_addr, he->h_addr, he->h_length);
	//
	//// Conectamos
	//if (connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr)) == -1) {
	//	return false;
	//}

	return true;
}


//-----------------------------------------------------------------------------
bool WinSocket::Listen(unsigned int port, int maxConnections) {
	//sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//if (sockfd < 0)
	//	return false;
	//
	//status = true;
	//
	//sockaddr.sin_family = AF_INET;
	//sockaddr.sin_port = htons(port);
	//
	//sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//
	//if (bind(sockfd,
	//	(struct sockaddr *)&sockaddr,
	//	sizeof(sockaddr)) < 0)
	//{
	//	return false;
	//}
	//
	//// Comenzamos la escucha
	//if (listen(sockfd, maxConnections) < 0)
	//{
	//	return false;
	//}

	return true;
}
std::shared_ptr<Socket> WinSocket::Accept() {

	//cerr << "accept()" << endl;
	//int sin_size = sizeof(struct sockaddr_in);
	////Revisar
	//auto socket_client = make_shared<WinSocket>();
	//
	//int sockfd_client = accept(sockfd, (struct sockaddr *)&(socket_client->sockaddr), &sin_size);
	//
	//socket_client->sockfd = sockfd_client;
	//// Corroboramos si no se cerró el socket
	//if (status != 1) return nullptr;
	//
	//return socket_client;
	return nullptr;

}
//-----------------------------------------------------------------------------
void WinSocket::deinit()
{
	this->status = false;
}
//-----------------------------------------------------------------------------
void WinSocket::Activate()
{
	this->status = true;
}

bool WinSocket::IsActive()
{
	return status;
}

long WinSocket::Recv(void* data, int dataLenght)
{
	//cerr << "receive()" << endl;
	//REVISAR
	//memset(data, '\0', dataLenght);
	// Recibimos datos en buffer
	//return recv(sockfd,(char*)data, dataLenght, 0);
	return 0;
}

long WinSocket::Send(const void* data, int dataLenght)
{
	////cerr << "send()" << endl;
	//// Cantidad de bytes que han sido enviados
	//int total_bytes = 0;
	//// Cantidad de bytes que faltan enviar
	//int residuary_bytes = dataLenght;
	//// Variable auxiliar
	//int n;
	//
	//while (residuary_bytes > 0)
	//{
	//	// Realizamos envío de bytes
	//	n = send(sockfd, (char *)data + total_bytes, residuary_bytes, 0);
	//
	//	if (n == -1)	return -1;
	//
	//	total_bytes += n;
	//	residuary_bytes -= n;
	//}
	//
	return 0;

}
