#pragma once
#include <chrono>
#include "Node.h"
#include "errorType.h"
#include "manifestHandler.h"
#include "sSocket.h"
#include "blockChain.h"
#include "layout.h"

typedef enum {IDLE, WAITING_LAYOUT, COLLECTING_MEMBERS, SENDING_LAYOUT, NETWORK_CREATED} fullNodeStates;


class FULLNode :
	public Node
{
public:
	FULLNode(NodeData ownData) : Node(ownData) {};
	~FULLNode();
	virtual void cycle();

	/***********************************************************************************
		METHODS USED BY CONTROLLER
	***********************************************************************************/
	errorType makeTX(const vector<Vout>& receivers);
	errorType makeBlock(); //"MINAR"
	errorType addNeighbour(NodeData neighbour); //agrega fulls

	/***********************************************************************************
		METHODS USED BY VIEWER
	***********************************************************************************/

	NodeData getData(); //own nodeData
	const vector<NodeData>* getNeighbours();
	const vector<Transaction>* getPendingTX();

private:
	/***********************************************************************************
		INNER EDACoin VARIABLES
	***********************************************************************************/
	BlockChain blockChain;											//THE BlockChain
	void saveBlockChain(BlockChain& blockchain, string path);		//Sve blockChain from .json file
	vector<string> filters;											//List of inner filters
	vector<Transaction> txs;										//List of transactions
	vector<MerkleBlock> merkleBlocks;								//List of merkleBlocks (?)
	void addBlock(Block block);										//Add a block to blockChain
	void saveTx(string trans);										//Save received transaction, add to tx list
	void saveMerkleBlock(string merkleBlock);						//Save received merkleBlock, add to merkleBlock list
	/***********************************************************************************
		NEIGHBOURHOOD AND NODES
	***********************************************************************************/
	vector<sSocket> neighbourhood;									//List of neighbour nodes
	Layout layout;
	/***********************************************************************************
		NETWORKING SH*T
	***********************************************************************************/
	fullNodeStates nodeState;
	vector <Server*> servers;								//Server List
	vector <Client*> clients;								//Client Lis
	unsigned int port;										//Just port number
	void keepListening();									//Make all available servers keep receiving messages, parse them if completed. Make last Server listen to void.
	void keepSending();										//All active clients keep sending their messages, parse if completed, destroy if completed.
	chrono::system_clock::time_point clock;					//Current time variable, used in initialization of Network Layout
	chrono::duration<int, milli> timeout;					//Time before TIMEOUT, randomly chosen on constructor
	//Json creation for network usage
	string createJsonBlock(unsigned int height);
	string createJsonTx(Transaction tx);
	string createJsonMerkle();
	string createJsonFilter(string filter);
	string createHeader(unsigned int height);
	//Response creation for newtwork usage
	string serverResponse(STATE rta);
	string createServerErrRsp();
	string createServerBlock(string path);
	string createServerOkRsp(string path);
	void createDates(char*, char*);
};

