/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "SPVNode.h"

/*******************************************************************************
	CONSTRUTOR
 ******************************************************************************/
SPVNode::SPVNode(NodeData ownData, NodeData filterNode, NodeData headerNode) {

}

/*******************************************************************************
	DESTRUCTOR
 ******************************************************************************/
SPVNode::~SPVNode() {

}

/*******************************************************************************
	CYCLE
 ******************************************************************************/
void SPVNode::cycle() {
	keepSending();
	keepListening();
}


/***********************************************************************************
	METHODS USED BY CONTROLLER
***********************************************************************************/
errorType SPVNode::postFilter()
{
	Client* client = new Client(filterNode);
	string id = JSONHandler.createJsonFilter(filterNode.getID());
	client->POST("/eda_coin/send_filter", id);
	errorType err = client->sendRequest();
	clients.push_back(client);
	notifyAllObservers();
	return err;
}

errorType SPVNode::changeFilterNode(NodeData FilterNode) { filterNode = FilterNode; }
errorType SPVNode::changeHeaderNode(NodeData HeaderNode) { headerNode = HeaderNode; }

/***********************************************************************************
	METHODS USED BY VIEWER
***********************************************************************************/
NodeData SPVNode::getData(){ return ownData; }
NodeData SPVNode::getFilterNodeData() { return filterNode; }
NodeData SPVNode::getHeaderNodeData() { return headerNode; }