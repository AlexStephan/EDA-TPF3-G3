#pragma once
#include "Node.h"

class SPVNode :
	public Node
{
public:
	SPVNode(NodeData ownData, NodeData FilterNode, NodeData HeaderNode) :
		Node(ownData), filterNode(FilterNode), headerNode(HeaderNode) {}
	virtual void cycle();
private:
	NodeData filterNode;
	NodeData headerNode;
};

