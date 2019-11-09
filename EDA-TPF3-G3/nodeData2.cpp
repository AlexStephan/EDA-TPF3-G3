#include "nodeData2.h"

nodeData2::nodeData2(uint port, ip_t ip) :
	port(port), ip(ip)
{
}

nodeData2::nodeData2(uint port, uint ip1, uint ip2, uint ip3, uint ip4) :
	port(port), ip({ip1,ip2,ip3,ip4})
{
}

nodeData2::nodeData2(nodeData2& c) :
	port(c.port), ip(c.ip)
{
}

string nodeData2::getPortString()
{
	return to_string(port);
}

string nodeData2::getIPString()
{
	return (to_string(ip.b1) + '.' + to_string(ip.b2) + '.' + to_string(ip.b3) + '.' + to_string(ip.b4));
}

uint nodeData2::getPort()
{
	return port;
}

ip_t nodeData2::getIP()
{
	return ip;
}
