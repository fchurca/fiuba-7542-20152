#include <cstring>

#include "socket.h"

#include "../model/charnames.h"

using namespace charnames;

#include <iostream>

using namespace std;

Socket::Socket() : inBufferIndex(0) {
}

Socket::~Socket() {
}

bool Socket::flushIn() {
	size_t oldSize = inBuffer.size();
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
		} else {
			cont = false;
			if (size < 0) {
				status = false;
				cerr << "Error in connection!";
			} else {
				cerr << "Empty partial";
			}
		}
		cerr << endl;
	} while (false); // FIXME
	return oldSize < inBuffer.size();
}

bool Socket::oFlushIn() {
	bool ret = true;
	if (inBufferIndex >= inBuffer.size()) {
		inBuffer.clear();
		ret = flushIn();
		inBufferIndex = 0;
	}
	return ret;
}

void Socket::flushOut() {
	*this << eot;
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
	uint32_t n = htonl(l);
	for(auto i = sizeof(n); i > 0; i--) {
		char c = n;
		*this << c;
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

Socket& Socket::operator>>(char& c) {
	if(oFlushIn()) {
		c = inBuffer.data()[inBufferIndex++];
	}
	return *this;
}

Socket& Socket::operator>>(long& l) {
	if(oFlushIn()) {
		uint32_t ret = 0;
		char c;
		for(auto i = 0; i < sizeof(ret); i++) {
			*this >> c;
			ret |= ((uint32_t)c << (8 * i));
		}
		l = ntohl(ret);
	}
	return *this;
}

Socket& Socket::operator>>(size_t& s) {
	long l;
	*this >> l;
	s = (size_t)l;
	return *this;
}

Socket& Socket::operator>>(string& s) {
	s = "";
	char c;
	do {
		*this >> c;
		if(c) {
			s += c;
		}
	} while (c);
	return *this;
}

