#pragma once
#include "Node.h"

#include "manifestHandler.h"
class FULLNode :
	public Node
{
public:
	FULLNode(NodeData ownData) : Node(ownData) {}
	virtual void cycle();
};

