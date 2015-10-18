#include <iostream>

#include "logger.h"

using namespace std;

Logger * Logger::instance = nullptr;

Logger::Logger(){
	enabledError = true;
	enabledWarning = true;
	enabledInformation = true;
	atexit(Logger::destroyInstance);
}

Logger::~Logger(){
	if (instance){
		instance->writeInformation("Destroying Logger");
		instance->close();
	}
}

//Singleton
Logger * Logger::getInstance() {
	if (!instance){
		instance = new Logger();
		instance->open();
	}
	return instance;
}

void Logger::destroyInstance() {
	if (instance) {
		delete (Logger::getInstance());
		instance = nullptr;
	}
}
/**
*	Establece nivel de log
*/
void Logger::setLevel(int level){
	enabledError = (level >= LOG_LEVEL_ERROR);
	enabledWarning =  (level >= LOG_LEVEL_WARNING);;
	enabledInformation =  (level >= LOG_LEVEL_INFORMATION);;
}

void Logger::flush() {
	if(file.is_open()) {
		file.flush();
	}
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
	std::string dia = getCurrentTime();
	std::replace(dia.begin(), dia.end(), ':', '-');
	string filename = "LOG " + dia + ".log";
	cerr << "Logging at " << filename << endl;
	file.open(filename, ios_base::out);
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
	if( enabledError )
		return write(message, "ERROR  ");
	return 0;
}
/**
*	Escribe en el archivo de log el mensaje recibido por parametro
*	@return: Devuelve 1 si se produjo algun error, 0 en caso contrario
*/
int Logger::writeWarning(string message){
	if( enabledWarning )
		return write(message, "WARNING");
	return 0;
}

/**
*	Escribe en el archivo de log el mensaje recibido por parametro
*	@return: Devuelve 1 si se produjo algun error, 0 en caso contrario
*/
int Logger::writeInformation(string message){
	if( enabledInformation )
		return write(message, "INFO   ");
	return 0;
}

/**
*	@return: Devuelve descripcion del error
*/
string Logger::getMessageError(){
	return messageError;
}
