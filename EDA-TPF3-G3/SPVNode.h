#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Node.h"
#include "blockChain.h"
#include "jsonHandler.h"

class SPVNode :
	public Node
{
public:
	SPVNode(NodeData ownData, NodeData FilterNode, NodeData HeaderNode);
	virtual void cycle();

	/***********************************************************************************
		METHODS USED BY CONTROLLER
	***********************************************************************************/
	errorType makeTX(const vector<Vout>& receivers, const vector<Vin>& givers);
	errorType postFilter();
	errorType changeFilterNode(NodeData FilterNode);
	errorType changeHeaderNode(NodeData HeaderNode);

	/***********************************************************************************
		METHODS USED BY VIEWER
	***********************************************************************************/
	NodeData getData();
	NodeData getFilterNodeData();
	NodeData getHeaderNodeData();
	errorType getVerificationError();

private:
	/***********************************************************************************
		NEIGHBOURHOOD AND NODES
	***********************************************************************************/
	NodeData filterNode;
	NodeData headerNode;
	vector<MerkleBlock> mBlocks;
	jsonHandler JSONHandler;
	vector<errorType> blockVerification;

	/***********************************************************************************
		NETWORKING SH*T
	***********************************************************************************/
	vector <Client*> clients;
	vector <Server*> servers;
	errorType getBlockHeader(string id);
	errorType postTransaction(Transaction tx);
	void verifyMerkleBlock();
	void keepListening();
	void keepSending();

	//SERVER RESPONSE
	string serverResponse(STATE rta, string massage);
	string createServerOkRsp(string path);
	string createServerErrRsp();
	void createDates(char* c1, char* c2);
};