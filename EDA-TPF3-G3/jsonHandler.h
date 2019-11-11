#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <iostream>
#include "Socket.h"
#include "blockChain.h"
#include "errorType.h"
#include "layout.h"

/*******************************************************************************
 * NAMESPACES
 ******************************************************************************/
using namespace std;

class jsonHandler
{
public:
	/***********************************************************************************
		SAVING METHODS
	***********************************************************************************/
	void saveBlockChain(BlockChain& blockchain, string path);
	void saveTx(string trans, vector<Transaction>& txs);
	void saveMerkleBlock(string merkleBlock,vector <MerkleBlock>& mrkl);

	/***********************************************************************************
		JSONS's CREATION
	***********************************************************************************/
	//Blockchain related
	string createJsonBlockHeader(BlockChain blckchain, string id);
	string createJsonBlockchain(BlockChain blckchain);
	string createJsonBlock(Block block);
	string createJsonTx(Transaction tx);
	string createJsonMerkle(Block block);
	string createJsonFilter(string id);

	//Server response related
	string createHeader(string id);
	string createJsonOk();
	string createJsonErr();
	string createJsonReady(Layout layout, BlockChain blockchain);
	string createJsonNotReady();

	/***********************************************************************************
		JSONS's VALIDATION
	***********************************************************************************/
	errorType validateBlock(string blck);
	errorType validateTx(string tx);
	errorType validateFilter(string filter);

	/***********************************************************************************
		LAYOUT CONTROLLERS
	***********************************************************************************/
	string createJsonLayout(Layout& layout);
	void readLayout(string layout, NodeData mySockt, vector <NodeData>& neighbourhood);
	void getNodesInLayout(string path, NodeData ownData, vector<NodeData>& nodes);


private:
	ip_t crackIp(string ip);
};