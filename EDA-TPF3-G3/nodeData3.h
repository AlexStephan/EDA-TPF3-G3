#pragma once
#include "nodeData2.h"
class nodeData3 :
	public nodeData2
{
	nodeData3(string id, uint port, ip_t ip);
	nodeData3(string id, uint port, uint ip1, uint ip2, uint ip3, uint ip4);
	nodeData3(string id, nodeData2& c);
	nodeData3(nodeData3& c);

	string getid();

protected:
	string id;
};

