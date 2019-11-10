#pragma once
#include "Socket.h"
#include <vector>
using namespace std;

struct Connection {
	string myId;
	Socket mySocket;
	vector<Socket> myNeighbours;
};

typedef vector<Connection> Layout;