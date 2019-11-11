#pragma once
#include "NodeData.h"
#include <vector>
using namespace std;

struct Connection {
	NodeData ownData;
	vector<NodeData> myNeighbours;
};

typedef vector<Connection> Layout;