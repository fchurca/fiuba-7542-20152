#include <cstring>

#include "socket.h"

#include "../model/charnames.h"

using namespace charnames;
using namespace std;

const size_t double_discretization_scale = 100;

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
			inBuffer.insert(inBuffer.end(), b, b + size);
		} else {
			cont = false;
			if (size < 0) {
				deinit();
			}
		}
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

bool Socket::flushOut() {
	*this << eot;
	auto oldSize = outBuffer.size();
	auto sent = Send((void *)outBuffer.data(), oldSize);
	outBuffer.clear();
	return sent == oldSize;
}

Socket& Socket::operator<<(char c) {
	outBuffer.push_back(c);
	return *this;
}

Socket& Socket::operator<<(bool b) {
	return *this << (b?(char)0xff:nul);
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
	return *this;
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
	return *this << (int)(d * double_discretization_scale);
}

Socket& Socket::operator>>(char& c) {
	if(oFlushIn()) {
		c = inBuffer.data()[inBufferIndex++];
	}
	return *this;
}

Socket& Socket::operator>>(bool& b) {
	char c = nul;
	*this >> c;
	b = (c != nul);
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

Socket& Socket::operator>>(int& i) {
	long l;
	*this >> l;
	i = (int)l;
	return *this;
}

Socket& Socket::operator>>(double& d) {
	long l;
	*this >> l;
	d = ((double)l) / double_discretization_scale;
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

