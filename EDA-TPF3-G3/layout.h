#pragma once
#include "Socket.h"
#include <vector>
using namespace std;

struct Connection {
	NodeData ownData;
	vector<NodeData> myNeighbours;
};

typedef vector<Connection> Layout;