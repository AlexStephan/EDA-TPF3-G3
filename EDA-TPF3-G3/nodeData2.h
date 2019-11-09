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


class nodeData2
{
public:
	nodeData2(uint port,ip_t ip);
	nodeData2(uint port, uint ip1, uint ip2, uint ip3, uint ip4);
	nodeData2(nodeData2& c);

	string getPortString();
	string getIPString();
	uint getPort();
	ip_t getIP();
	
protected:
	uint port;
	ip_t ip;
};

