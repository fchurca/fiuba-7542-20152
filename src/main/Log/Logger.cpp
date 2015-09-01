#include "Logger.h"
#include <cstring>

Logger::Logger(){
}

Logger::~Logger(){
}

Logger * Logger::instance = 0;
//Singleton
Logger * Logger::getInstance() {
	if (!instance){
		instance = new Logger();
		instance->isNew = true;
	}
	return instance;
}

/**
 *	@return: Fecha y Hora actual
 */
string Logger::getCurrentTime(){
	time_t now = time(0);
	struct tm  tstruct= *localtime(&now);
	char buf[80];
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return buf;
}

/**
 *	Escribe en el archivo de log el mensaje recibido por parametro
 *	@return: Devuelve 1 si se produjo algun error, 0 en caso contrario
 */
int Logger::write(string message, string tipo){
	ofstream file;

	if(isNew){
		file.open("LOG.log", ios_base::out);
		isNew = false;
	}
	else{
		file.open("LOG.log", ios_base::app);
	}
	
	try {
		if (!file){	
			messageError = "No se ha podido abrir el archivo";
			return 1;
		}
		file << getCurrentTime() << " - "<< tipo << " - " << message << "\n" ;
		file.close();
		return 0;
	}
	catch(exception &e) {
		messageError = strcat("ERROR : ", e.what()); 
		return 1;
	}
};

/**
 *	Escribe en el archivo de log el mensaje recibido por parametro
 *	@return: Devuelve 1 si se produjo algun error, 0 en caso contrario
 */
int Logger::writeError(string message){
	return write(message, "ERROR  ");
}
/**
 *	Escribe en el archivo de log el mensaje recibido por parametro
 *	@return: Devuelve 1 si se produjo algun error, 0 en caso contrario
 */
int Logger::writeWarning(string message){
	return write(message, "WARNING");
}

/**
 *	Escribe en el archivo de log el mensaje recibido por parametro
 *	@return: Devuelve 1 si se produjo algun error, 0 en caso contrario
 */
int Logger::writeInformation(string message){
	return write(message, "INFO   ");
}

/**
 *	@return: Devuelve descripcion del error
 */
string Logger::getMessageError(){
	return messageError;
}
