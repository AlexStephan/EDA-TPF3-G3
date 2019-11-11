#pragma once
#include "NodeData.h"
#include <vector>
using namespace std;

struct Connection {
	Connection(const NodeData& ownData) : ownData(ownData), myNeighbours() {}
	NodeData ownData;
	vector<NodeData> myNeighbours;
};

typedef vector<Connection> Layout;