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
	//NOTA: seria super chevere si se modificaran los metodos de Block
	//para hacerlos const, asi la siguiente funcion es libre de devolver
	//referencia a const Blockchain, y no a simplemente blockchain (si
	//no hay getters const, no puedo acceder a los getters desde un
	//objeto const !!!! Si durante el proceso de volver const los
	//getters de Block tropezaron con alguna definicion que yo (Alex)
	//hice -por imprudencia propia- avisarme y lo arreglo
	BlockChain* getBlockChain();

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
	//INTERACTION WITH STRANGERS
	errorType postLayout(Socket socket);
	errorType postPing(Socket socket);
	//INTERACTION WITH NEIGHBOURS
	string serverResponse(STATE rta);
	errorType postBlock(unsigned int neighbourPos, unsigned int height);
	errorType getBlockHeader(unsigned int height, unsigned int neighbourPos);
	errorType postTransaction(unsigned int neighbourPos, Transaction tx);
	errorType postMerkleBlock(unsigned int neighbourPos);
	errorType postFilter(unsigned int neighbourPos);
};

