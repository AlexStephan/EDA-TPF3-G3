#include "NodeData.h"

NodeData::NodeData(string id, uint port, ip_t ip) :
	id(id), socket(port,ip)
{
}

NodeData::NodeData(string id, uint port, uint ip1, uint ip2, uint ip3, uint ip4):
	id(id), socket(port,ip1,ip2,ip3,ip4)
{
}

NodeData::NodeData(string id, Socket& c) :
	id(id), socket(c)
{
}

NodeData::NodeData(const NodeData& c) :
	id(c.id), socket(c.socket)
{
}

string NodeData::getID()
{
	return id;
}

Socket NodeData::getSocket()
{
	return socket;
}
