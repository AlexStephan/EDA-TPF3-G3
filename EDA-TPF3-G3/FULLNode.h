//       \\\\\           \\\\\           \\\\\           \\\\\    
//      \\\\\\\__o      \\\\\\\__o      \\\\\\\__o      \\\\\\\__o
//______\\\\\\\'/_______\\\\\\\'/_______\\\\\\\'/_______\\\\\\\'/_
//private->protected


#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <chrono>
#include "Node.h"
#include "errorType.h"
#include "manifestHandler.h"
#include "blockChain.h"
#include "layout.h"
#include"jsonHandler.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 *****************************************************************************/
typedef enum {IDLE, WAITING_LAYOUT, COLLECTING_MEMBERS, SENDING_LAYOUT,GOT_LAYOUT, NETWORK_CREATED} fullNodeStates;


class FULLNode :
	public Node
{
public:
	FULLNode(NodeData ownData);
	~FULLNode();
	virtual void cycle();

	/***********************************************************************************
		METHODS USED BY CONTROLLER
	***********************************************************************************/
	errorType makeTX(const vector<Vout>& receivers, const vector<Vin>& givers);
	errorType makeBlock(); //"MINAR"
	errorType addNeighbour(NodeData neighbour); //agrega fulls

	/***********************************************************************************
		METHODS USED BY VIEWER
	***********************************************************************************/
	NodeData getData(); //own nodeData
	const vector<NodeData>* getNeighbours();
	const vector<Transaction>* getPendingTX();
	const BlockChain* getBlockChain();
	const Layout* getLayout();
	fullNodeStates getState();

protected:
	/***********************************************************************************
		INNER EDACoin VARIABLES
	***********************************************************************************/
	BlockChain blockChain;											//THE BlockChain
	vector<Filter> filters;											//List of inner filters
	vector<Transaction> txs;										//List of transactions
	vector<MerkleBlock> merkleBlocks;								//List of merkleBlocks (?)
	void addBlock(Block block);										//Add a block to blockChain
	void addTx(string trans);										//Save received transaction, add to tx list
	/***********************************************************************************
		NEIGHBOURHOOD AND NODES
	***********************************************************************************/
	vector<NodeData> neighbourhood;									//List of neighbour nodes
	vector<NodeData> network;
	vector<NodeData> askedForFilter;
	Layout layout;													//Layout in vector<connection> form
	string layoutMsg;												//Layout in JSON form
	vector<NodeData> nodesInManifest;
	/***********************************************************************************
		NETWORKING SH*T
	***********************************************************************************/
	void makeLayout(); //me apodere de la declaracion : parametros: ownData, network, layout
	fullNodeStates nodeState;
	vector <Server*> servers;								//Server List
	vector <Client*> clients;								//Client Lis
	static bool isLedaderNode; 
	unsigned int port;										//Just port number
	void keepListening();									//Make all available servers keep receiving messages, parse them if completed. Make last Server listen to void.
	void keepSending();										//All active clients keep sending their messages, parse if completed, destroy if completed.
	chrono::system_clock::time_point clock;					//Current time variable, used in initialization of Network Layout
	chrono::duration<int, milli> timeout;					//Time before TIMEOUT, randomly chosen on constructor

	//CLIENT POST
	errorType postMerkleBlock(Block blck, Transaction tx, unsigned int neighbourPos);
	errorType postMerkleBlock(Block blck, Transaction tx, NodeData data);
	errorType postTransaction(unsigned int neighbourPos, Transaction tx);
	errorType postBlock(unsigned int neighbourPos, unsigned int height);
	errorType postLayout(NodeData data);
	errorType postPing(NodeData data);

	//SERVER RESPONSE
	string serverResponse(STATE rta, string massage);
	string createServerOkRsp(string path);
	string createServerHeader(string path, string id);
	string createServerErrRsp();
	string createServerReadyRsp();
	string createServerNotReadyRsp();
	void createDates(char* c1, char* c2);
	jsonHandler JSONHandler;
	void manageNetworkReady(string rta);

	/***********************************************************************************
		FLOODING / VERIFICATION
	***********************************************************************************/
	void checkForFilter(Block blck);
	bool checkForId(string id);
	void floodBlock(Block blck, NodeData sender);
	void floodTx(Transaction tx, NodeData sender);
};