#pragma once

//NOTA: favor de pasar todas las defs. q vean en el .h al .cpp

#include "subject.h"
#include "NodeData.h"

//usar vector o lo q consideren necesario
#include "server.h"
#include "client.h"

//
#include "errorType.h"

class Node :
	public Subject
{
public:
	Node(NodeData data) : ownData(data), :myMoney(0) {}
	NodeData getData() { return ownData; } //ID, IP y Puerto
	longN getMyMoney() { return myMoney; }
	virtual void cycle() = 0;

protected:
	NodeData ownData;
	longN myMoney;
};

