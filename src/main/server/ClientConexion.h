//-----------------------------------------------------------------------------
#ifndef SRC_MAIN_SERVER_CLIENTCONEXION_H_
#define SRC_MAIN_SERVER_CLIENTCONEXION_H_
//-----------------------------------------------------------------------------
class ClientConexion {
public:
	ClientConexion(Socket* socket);
	virtual ~ClientConexion();

private:
	Socket *socket;
};
//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_SERVER_CLIENTCONEXION_H_ */
//-----------------------------------------------------------------------------
