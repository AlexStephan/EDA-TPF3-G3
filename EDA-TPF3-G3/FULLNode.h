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
	vector<string> filters;											//List of inner filters
	vector<Transaction> txs;										//List of transactions
	vector<MerkleBlock> merkleBlocks;								//List of merkleBlocks (?)
	void addBlock(Block block);										//Add a block to blockChain
	void addTx(string trans);										//Save received transaction, add to tx list
	/***********************************************************************************
		NEIGHBOURHOOD AND NODES
	***********************************************************************************/
	vector<NodeData> neighbourhood;									//List of neighbour nodes
	vector<NodeData> network;
	Layout layout;													//Layout in vector<connection> form
	string layoutMsg;												//Layout in JSON form
	vector<NodeData> nodesInManifest;
	/***********************************************************************************
		NETWORKING SH*T
	***********************************************************************************/
	void makeLayout(NodeData own, vector<NodeData> network, Layout& layout);
	fullNodeStates nodeState;
	vector <Server*> servers;								//Server List
	vector <Client*> clients;								//Client Lis
	unsigned int port;										//Just port number
	void keepListening();									//Make all available servers keep receiving messages, parse them if completed. Make last Server listen to void.
	void keepSending();										//All active clients keep sending their messages, parse if completed, destroy if completed.
	chrono::system_clock::time_point clock;					//Current time variable, used in initialization of Network Layout
	chrono::duration<int, milli> timeout;					//Time before TIMEOUT, randomly chosen on constructor
	//INTERACTION WITH STRANGERS
	errorType postLayout(Socket socket);
	errorType postTransaction(unsigned int neighbourPos, Transaction tx);
	errorType postBlock(unsigned int neighbourPos, unsigned int height);
	errorType postPing(Socket socket);
	//INTERACTION WITH NEIGHBOURS
	string serverResponse(STATE rta);
};

