#pragma once
#include "Node.h"
#include "block.h"

class SPVNode :
	public Node
{
public:
	SPVNode(NodeData ownData, NodeData FilterNode, NodeData HeaderNode) :
		Node(ownData), filterNode(FilterNode), headerNode(HeaderNode) {}
	~SPVNode();
	virtual void cycle();

/***********************************************************************************
	METHODS USED BY CONTROLLER
***********************************************************************************/
	errorType makeTX(const vector<Vout>& receivers);
	errorType postFilter();
	errorType changeFilterNode(NodeData FilterNode);
	errorType changeHeaderNode(NodeData HeaderNode);

/***********************************************************************************
	METHODS USED BY VIEWER
***********************************************************************************/
	NodeData getData();
	NodeData getFilterNodeData();
	NodeData getHeaderNodeData();

private:
	NodeData filterNode;
	NodeData headerNode;
/***********************************************************************************
	NETWORKING SH*T
***********************************************************************************/
	vector <Server*> servers;								//Server List
	vector <Client*> clients;								//Client Lis
	unsigned int port;										//Just port number
	void keepListening();									//Make all available servers keep receiving messages, parse them if completed. Make last Server listen to void.
	void keepSending();										//All active clients keep sending their messages, parse if completed, destroy if completed.
	//Json creation for network usage
	string createJsonFilter(string filter);
	string createHeader(unsigned int height);
	//Response creation for newtwork usage
	string serverResponse(STATE rta);
	string createServerErrRsp();
	string createServerOkRsp(string path);
	void createDates(char*, char*);

};

