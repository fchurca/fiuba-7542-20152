#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string> 
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>

#include "../defines/defines.h"

class Logger{
private:
	static Logger * instance;
	bool enabledError;
	bool enabledWarning;
	bool enabledInformation;
	std::ofstream file;
	std::string messageError;
	std::string getCurrentTime();
	int write(std::string message, std::string tipo);
	void open();
	void close();
	Logger();
public:
	static Logger * getInstance();
	static void destroyInstance();
	void setLevel(int level);
	void flush();
	int writeError(std::string message);
	int writeWarning(std::string message);
	int writeInformation(std::string message);
	std::string getMessageError();
	~Logger();
};

#endif //__LOGGER_H__
