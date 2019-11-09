#include "nodeData3.h"

nodeData3::nodeData3(string id, uint port, ip_t ip) :
	id(id), nodeData2(port,ip)
{
}

nodeData3::nodeData3(string id, uint port, uint ip1, uint ip2, uint ip3, uint ip4) :
	id(id), nodeData2(port,ip1,ip2,ip3,ip4)
{
}

nodeData3::nodeData3(string id, nodeData2& c) :
	id(id), nodeData2(c)
{
}

nodeData3::nodeData3(nodeData3& c) :
	id(c.id), nodeData2(c.port,c.ip)
{
}


string nodeData3::getid()
{
	return id;
}
