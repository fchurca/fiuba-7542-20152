#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string> 
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

class Logger{
private:
	static Logger * instance;
	bool isNew;
	string messageError;
	string getCurrentTime();
	int write(string message, string tipo);
	Logger();
public:
	static Logger * getInstance();
	int writeError(string message);
	int writeWarning(string message);
	int writeInformation(string message);
	string getMessageError();
	~Logger();
};

#endif //__LOGGER_H__