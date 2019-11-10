#pragma once
#include "Node.h"
#include "blockChain.h"

class SPVNode :
	public Node
{
public:
	SPVNode(NodeData ownData, NodeData FilterNode, NodeData HeaderNode) :
		Node(ownData), filterNode(FilterNode), headerNode(HeaderNode) {}
	virtual void cycle();

	//Metodos accedidos desde el controller
	errorType makeTX(const vector<Vout>& receivers);
	errorType postFilter();
	errorType changeFilterNode(NodeData FilterNode);
	errorType changeHeaderNode(NodeData HeaderNode);

	//Accedidas desde el viewer:
	NodeData getData();
	NodeData getFilterNodeData();
	NodeData getHeaderNodeData();

private:
	NodeData filterNode;
	NodeData headerNode;
};

