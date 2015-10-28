#include <cstring>

#include "socket.h"

#include "../model/charnames.h"

using namespace charnames;

#include <iostream>

using namespace std;

void Socket::flushIn() {
	inBuffer.clear();
	const size_t bufsize = 4097;
	char b[bufsize];
	long size;
	bool cont = true;
	do {
		memset(b, nul, bufsize);
		size = Recv((void *)b, bufsize-1);
		if(size > 0) {
			cerr << size << " bytes partial: `" << b << '`';
			inBuffer.insert(inBuffer.end(), b, b + size);
			cerr << ", last char is " << (int)b[size - 1];
			cont = b[size - 1] != ht;
		} else {
			cont = false;
			if (size < 0) {
				cerr << "Error in connection!";
			} else {
				cerr << "Empty partial";
			}
		}
		cerr << endl;
	} while (cont);
}

void Socket::flushOut() {
	outBuffer.push_back(eot);
	Send((void *)outBuffer.data(), outBuffer.size());
	outBuffer.clear();
}

Socket& Socket::operator<<(char c) {
	outBuffer.push_back(c);
	return *this;
}

Socket& Socket::operator<<(int i) {
	return *this<<(long)i;
}

Socket& Socket::operator<<(long l) {
	auto n = htonl(l);
	for(auto i = sizeof(n); i > 0; i--) {
		*this << (char)n;
		n >>= 8;
	}
}

Socket& Socket::operator<<(size_t s) {
	return *this<<(long)s;
}

Socket& Socket::operator<<(std::string s) {
	for(size_t i = 0; i < s.length(); i++) {
		*this<<s[i];
	}
	*this<<nul;
	return *this;
}

Socket& Socket::operator<<(double d) {
	return *this << (int)(d * 100);
}

