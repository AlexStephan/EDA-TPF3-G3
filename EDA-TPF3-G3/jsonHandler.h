#pragma once
#include <iostream>
#include "Socket.h"
#include "blockChain.h"
#include "errorType.h"
#include "layout.h"

using namespace std;

class jsonHandler
{
public:
	void saveBlockChain(BlockChain& blockchain, string path);
	void saveTx(string trans, vector<Transaction>& txs);
	void saveMerkleBlock(string merkleBlock,vector <MerkleBlock>& mrkl);


	string createJsonBlock(Block block);
	string createJsonTx(Transaction tx);
	string createJsonMerkle(Block block);
	string createJsonFilter(string id);
	string createHeader(string id);
	string createJsonLayout(Layout& layout);

	errorType validateBlock(string blck);
	errorType validateTx(string tx);
	errorType validateFilter(string filter);
	
	void readLayout(string layout, NodeData mySockt, vector <NodeData>& neighbourhood);
	void getNodesInLayout(string path, NodeData ownData, vector<NodeData>& nodes);

private:
	ip_t crackIp(string ip);
};