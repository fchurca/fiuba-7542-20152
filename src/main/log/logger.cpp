#include "logger.h"

Logger::Logger(){
}

Logger::~Logger(){
	if (instance){
		instance->writeInformation("Destroying Logger");
		instance->close();
	}
}

Logger * Logger::instance = 0;
//Singleton
Logger * Logger::getInstance() {
	if (!instance){
		instance = new Logger();
		instance->open();
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
*	Abre el archivo de log
*/
void Logger::open(){
	file.open("LOG.log", ios_base::out);
}

/**
*	Cierra el archivo de log 
*/
void Logger::close(){
	file.close();
}

/**
*	Escribe en el archivo de log el mensaje recibido por parametro
*	@return: Devuelve 1 si se produjo algun error, 0 en caso contrario
*/
int Logger::write(string message, string tipo){
	if (!file){	
		messageError = "No se ha podido abrir el archivo";
		return 1;
	}
	file << getCurrentTime() << " - "<< tipo << " - " << message << "\n" ;
	return 0;
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
