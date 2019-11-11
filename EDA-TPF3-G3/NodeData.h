#pragma once

#include "Socket.h"
class NodeData
{
public:
	NodeData(string id, uint port, ip_t ip);
	NodeData(string id, uint port, uint ip1, uint ip2, uint ip3, uint ip4);
	NodeData(string id, Socket& c);
	NodeData(const NodeData& c);

	string getID() const;
	Socket getSocket() const;

	NodeData& operator=(const NodeData& data);

private:
	string id;
	Socket socket;
};