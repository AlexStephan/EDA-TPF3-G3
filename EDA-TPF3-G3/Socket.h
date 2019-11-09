#pragma once

#include <string>

using namespace std;
typedef unsigned int uint;

struct ip_t {
	uint b1;
	uint b2;
	uint b3;
	uint b4;
};


class Socket
{
public:
	Socket(uint port, ip_t ip);
	Socket(uint port, uint ip1, uint ip2, uint ip3, uint ip4);
	Socket(Socket& c);

	string getPortString();
	string getIPString();
	uint getPort();
	ip_t getIP();

	bool operator==(Socket& s);

private:
	uint port;
	ip_t ip;
};



