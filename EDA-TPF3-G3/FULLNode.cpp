/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "FULLNode.h"
#include "layoutGeneratorHandler.h"

 /*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CRLF "\x0D\x0A"

/*******************************************************************************
	CONSTRUCTOR
 ******************************************************************************/
FULLNode::FULLNode(NodeData ownData) : Node(ownData) {
	nodeState = IDLE;
	JSONHandler.saveBlockChain(blockChain, "BlockChain.json");
	JSONHandler.getNodesInLayout("manifest.json", ownData, nodesInManifest);
	//Timing sh*t
	clock = chrono::system_clock::now();
	int timing = rand() % 1000 + 1;
	chrono::duration<int, milli> dur(timing);
	timeout = dur;
}

/*******************************************************************************
	DESTRUCTOR
 ******************************************************************************/
FULLNode::~FULLNode() {
	cout << "Destroyed node" << endl;

	if (!servers.empty()) {
		for (int i = 0; i < servers.size(); i++)
			delete servers[i];
		servers.clear();
	}
	if (!clients.empty()) {
		for (int i = 0; i < clients.size(); i++)
			delete clients[i];
		clients.clear();
	}
}

/*******************************************************************************
	CYCLE
 ******************************************************************************/
void FULLNode::cycle() {
	switch (nodeState) {
	case IDLE:
		if (servers.back()->getDoneListening()) {					//If latest server picks up a ping, we stop trying
			nodeState = WAITING_LAYOUT;
			break;
		}
		//Pick random timeout
		if (chrono::system_clock::now() > clock + timeout) {	//If timout ocurred
			nodeState = COLLECTING_MEMBERS;						//We take care of the layout
			for (int i = 0; i < nodesInManifest.size(); i++) {
				postPing(nodesInManifest[i]);		//Ping each node in manifest who isn't me (just a bit, rest of sending is done in COLLECTING_MEMBERS)
			}
		}
		break;
	case COLLECTING_MEMBERS:									//Look at me, I build the network now
		int gotReady = -1;
		for (int i = 0; i < clients.size(); i++) {
			Client* curr = clients[i];
			curr->sendRequest();
			if (curr->getRunning == 0)
				if (curr->getResponse() == MSG_NETWORK_READY) {							//SPEAK WITH NETWORKING PPL
					nodeState = SENDING_LAYOUT;
					gotReady = i;
					break;
				}
				else if (curr->getResponse() == MSG_NETWORK_NOT_READY) {				//SPEAK WITH NETWORKING PPL
					network.push_back(curr->getReceiverData());
					delete clients[i];								//Destroy client
					clients.erase(clients.begin() + i);				//Remove client from list
				}
				else{												//If none of the responses is received
					postPing(clients[i]->getReceiverData());				//Post ping again
					delete clients[i];								//Destroy failed client
					clients.erase(clients.begin() + i);				//Remove failed client from list
				}
		}
		if (nodesInManifest.size() == network.size()) {				//If the amount of nodes in the manifest is equal to the amount of nodes who responded NETWORK_NOT_READY
			bool allResponded = true;
			for (int i = 0; i < nodesInManifest.size(); i++) {
				bool found = false;
				for (int j = 0; j < network.size(); j++) {
					if (nodesInManifest[i].getID() == network[j].getID()  && nodesInManifest[i].getSocket().getIP() == network[j].getSocket().getIP() && nodesInManifest[i].getSocket().getPort() == network[j].getSocket().getPort())
						found = true;
				}
				if (!found)
					allResponded = false;
			}		//COMMENT DOUBLE FOR IF UNRESPONSIVE, JUST COMPARING SIZES ((?) MIGHT BE ENOUGH TO ENSURE CONNECTION
			if (allResponded)
				nodeState = SENDING_LAYOUT;
		}
		if (nodeState == SENDING_LAYOUT) {
			makeLayout();
			layoutMsg = JSONHandler.createJsonLayout(layout);
			if (gotReady != -1) {
				addNeighbour(clients[gotReady]->getReceiverData());						//SPEAK WITH NETWORKING PPL
			}
			for (int i = 0; i < clients.size(); i++) {
				delete clients[i];								//Destroy client
				clients.erase(clients.begin() + i);				//Remove client from list
			}
			clients.empty();
			for (int i = 0; i < network.size(); i++)
				postLayout(clients[i]->getReceiverData());				//Start posting Layout to all nodes in network
		}
		break;
	case WAITING_LAYOUT:
		if (!servers.back()->getDoneListening())
			servers.back()->listening();
		else if (!servers.back()->getDoneDownloading())
			servers.back()->receiveMessage();
		else if(!servers.back()->getDoneSending())
			servers.back()->sendMessage(serverResponse(servers.back()->getState()));
		if (servers.back()->getDoneSending()) {
			if (servers.back()->getState() == GOT_LAYOUT) {					//If layout was correctly received		//SPEAK WITH NETWORKING PPL
				JSONHandler.readLayout(servers.back()->getMessage(), ownData, neighbourhood);		//Read layout, and add my neighbours
				delete servers.back();
				servers.pop_back();											//Remove useless server
				Server* newServer = new Server(port);
				newServer->startConnection();								//Create new server
				servers.push_back(newServer);
				nodeState = NETWORK_CREATED;								//And now we work as usual
			}
			else {															//If layout wasn't correctly received
				delete servers.back();
				servers.pop_back();											//Remove useless server
				Server* newServer = new Server(port);
				newServer->startConnection();								//Create new server
				servers.push_back(newServer);
			}
		}
		break;
	case SENDING_LAYOUT:
		for (int i = 0; i < clients.size(); i++) {
			clients[i]->sendRequest();
			if(clients[i]->getRunning == 0)
				if (clients[i]->getResponse() == HTTP_OK) {					//SPEAK WITH NETWORKING PPL
					delete clients[i];								//Destroy client
					clients.erase(clients.begin() + i);				//Remove client from list
				}
				else {
					postLayout(clients[i]->getReceiverData());			//Post layout again
					delete clients[i];								//Destroy failed client
					clients.erase(clients.begin() + i);				//Remove failed client from list
				}
		}
		if (clients.empty())										//If all clients were destroyed
			nodeState = NETWORK_CREATED;							//All nodes received layout, and network is created
		break;
	case NETWORK_CREATED:
		keepListening();
		keepSending();
		break;
	default:
		cout << "You fucked up with the FSM, Morty!" << endl;
		break;
	}
}

/***********************************************************************************
		METHODS USED BY CONTROLLER
***********************************************************************************/
errorType FULLNode::makeTX(const vector<Vout>& receivers, const vector<Vin>& givers){
	Transaction tx;
	Block aux;
	//BUILD TX
	tx.vIn = givers;
	tx.nTxIn = givers.size();
	tx.vOut = receivers;
	tx.nTxOut = receivers.size();
	tx.txId = aux.getTxId(tx);
	for (int i = 0; i < neighbourhood.size(); i++)
		postTransaction(i, tx);												//Post Tx to all neighbours
	errorType ret;
	return ret;
}
errorType FULLNode::makeBlock() {
	errorType ret;
	Block block;
	//DO SOMETHING TO BLOCK
	block.setHeight(blockChain.size() + 1);
	//END OF BLOCK CONFIG
	blockChain.push_back(block);
	for (int i = 0; i < neighbourhood.size(); i++)
		postBlock(i, blockChain.size());									//Post Block to all neighbours
	errorType ret;
	return ret;
}
errorType FULLNode::addNeighbour(NodeData neighbour) {
	errorType ret;
	neighbourhood.push_back(neighbour);
	return ret;
}

/***********************************************************************************
		METHODS USED BY VIEWER
***********************************************************************************/
NodeData FULLNode::getData() {
	return ownData;
}
const vector<NodeData>* FULLNode::getNeighbours() {
	return &neighbourhood;
}
const vector<Transaction>* FULLNode::getPendingTX() {
	return &txs;
}

const BlockChain* FULLNode::getBlockChain() {
	return &blockChain;
}

const Layout* FULLNode::getLayout()
{
	return &layout;
}

fullNodeStates FULLNode::getState()
{
	return nodeState;
}


/***********************************************************************************
	INNER EDACoin VARIABLES
***********************************************************************************/
void FULLNode::addBlock(Block block) {
	blockChain.push_back(block);
	notifyAllObservers();
}

void FULLNode::addTx(string trans) {
	JSONHandler.saveTx(trans, txs);
	notifyAllObservers();
}
//void FULLNode::saveMerkleBlock(string merkleBlock) {}

/***********************************************************************************
	NETWORKING SH*T
***********************************************************************************/
void FULLNode::makeLayout()
{
	layoutGeneratorHandler layoutGen(ownData, network);
	layoutGen.generateLayout();
	layout = layoutGen.getLayout();
}
void FULLNode::keepListening() {
	vector<vector<Server*>::iterator> deleteThis;
	vector<Server*> doneServers;
	servers.back()->listening();

	if ((*(servers.end() - 1))->getDoneListening()) {
		cout << "Latest Server picked up something!" << endl;
		Server* newServer = new Server(port);
		newServer->startConnection();
		servers.push_back(newServer);
		cout << "New Server created and pushed!" << endl;
	}
	auto i = servers.begin();
	for (; i != servers.end() - 1; i++) {
		if (!(*i)->getDoneDownloading())
			(*i)->receiveMessage();
		else if (!(*i)->getDoneSending())
			(*i)->sendMessage(serverResponse((*i)->getState()));
		if ((*i)->getDoneSending()) {
			cout << "Server done servering" << endl;
			doneServers.push_back(*i);
			deleteThis.push_back(i);
		}
	}
	//Handle finished servers
	auto j = doneServers.begin();
	for (; j != doneServers.end(); j++) {
		Block blck;
		switch ((*j)->getState()) {
		case BLOCK:			//Done
			blck.saveBlock((*j)->getMessage());
			bool found = false;
			for (int i = 0; i < blockChain.size(); i++) {
				if (blck.getBlockID() == blockChain[i].getBlockID())			//If received block is already in chain, it gets ignored
					found = true;
			}
			blockChain.push_back(blck);										//Save block into blockchain
			floodBlock(blck, (*j)->getSender());							//And flood the block
			if (!found)														//If it's a new block
				checkForFilter(blck);											//Inform possible suscripted SPVNodes
			else
				blockChain.pop_back();										//Remove block if repeated
			break;
		case TX:			//Done
			JSONHandler.saveTx((*j)->getMessage(), txs);
			bool found = false;
			for (int i = 0; i < txs.size() - 1; i++) {
				if (txs.back().txId == txs[i].txId)
					found = true;
			}
			if(found)														
				blockChain.pop_back();										//Remove tx if repeated
			floodTx(txs.back(), (*j)->getSender());							//And flood the tx
			break;
		case MERKLE:														//FULL NODES DONT CARE ABOUT RECEIVING MERKLE BLOCKS
			break;
		case FILTER:		
			filters.push_back((*j)->getMessage());
			break;
		}
	}
	i = servers.begin();
	for (; i != servers.end() - 1; i++) {
		if ((*i)->getDoneSending())
			delete* i;
	}
	auto k = deleteThis.begin();
	for (; k != deleteThis.end(); k++) {
		servers.erase(*k);
	}
	if (!deleteThis.empty())
		notifyAllObservers();
}

void FULLNode::keepSending() {
	if (clients.empty())
		return;
	vector<vector<Client*>::iterator> deleteThis;
	vector<Client*> doneClients;

	auto i = clients.begin();
	for (; i != clients.end(); i++) {
		if ((*i)->getRunning() == 0) {
			cout << "Client did it's job!" << endl;
			doneClients.push_back(*i);
			deleteThis.push_back(i);
		}
		else
			(*i)->sendRequest();
	}
	//Handle finished clients
	auto j = doneClients.begin();
	for (; j != doneClients.end(); j++) {
		/*//Parse their msgs
		if ((*j)->getClientType() == POSTClient) {

		}
		else if ((*j)->getClientType() == GETClient) {
			saveMerkleBlock((*j)->getResponse());
		}*/														//FULLNODES DONT EXPECT ANSWER TO POSTS IN NETWORK_READY STATE (?)
		//And delete them
		delete* j;
	}
	auto k = deleteThis.begin();
	for (; k != deleteThis.end(); k++) {
		clients.erase(*k);
	}
	if (!deleteThis.empty())
		notifyAllObservers();
}

/***********************************************************************************
	POSTING / GETTING METHODS
***********************************************************************************/
errorType FULLNode::postTransaction(unsigned int neighbourPos, Transaction tx)
{
	Client* client = new Client(neighbourhood[neighbourPos]);
	string tx_ = JSONHandler.createJsonTx(tx);
	cout << "JSON:" << endl << tx_ << endl;	//DEBUG
	client->POST("/eda_coin/send_tx", tx_);
	errorType err = client->sendRequest();
	clients.push_back(client);

	notifyAllObservers();
	return err;
}

errorType FULLNode::postBlock(unsigned int neighbourPos, unsigned int height)
{
	Block bl0ck;
	for (int i = 0; i < blockChain.size(); i++)
	{
		blockChain[i].getHeight = height;
		{
			bl0ck = blockChain[i];
		}
	}

	Client* client = new Client(neighbourhood[neighbourPos]);
	string blck = JSONHandler.createJsonBlock(bl0ck);
	client->POST("/eda_coin/send_block", blck);
	errorType err = client->sendRequest();
	clients.push_back(client);

	notifyAllObservers();
	return err;
}


errorType FULLNode::postLayout(NodeData sock)
{
	Client* client = new Client(sock);
	client->POST("/eda_coin/NETWORK_LAYOUT", layoutMsg);
	errorType err = client->sendRequest();
	clients.push_back(client);

	notifyAllObservers();
	return err;
}



errorType FULLNode::postPing(NodeData sock)
{
	Client* client = new Client(sock);
	client->POST("/eda_coin/PING");
	errorType err = client->sendRequest();
	clients.push_back(client);

	notifyAllObservers();
	return err;
}

/***********************************************************************************
	SEREVR REPONSE
***********************************************************************************/
string FULLNode::serverResponse(STATE rta, string msg)
{
	string message;

	switch (rta)
	{
	case HEADER:
		if (checkForId(msg))
		{
			message = createServerHeader("/eda_coin/get_block_header", msg);
		}

		else
		{
			message = createServerErrRsp();
		}
		break;

	case TX:
		message = createServerOkRsp("/eda_coin/send_tx");
		break;

	case BLOCK:
		message = createServerOkRsp("/eda_coin/send_block");
		break;

	case MERKLE:
		message = createServerOkRsp("/eda_coin/send_merkle_block");
		break;

	case FILTER:
    	message = createServerOkRsp("/eda_coin/send_filter");
		break;

	case LAYOUT:
		message = createServerOkRsp("/eda_coin/send_filter");
		break;

	case PING:
		if (nodeState == NETWORK_CREATED)	//DUDOSISIMO ESPERAR ALEJO
		{
			message = createServerReadyRsp();
		}

		else
		{
			message = createServerNotReadyRsp();
		}
		break;

	case ERR:
		message = createServerErrRsp();
		break;
	}

	return message;

}

string FULLNode::createServerNotReadyRsp()
{
	string message;
	char dateLine[100];
	char expiresLine[100];
	createDates(dateLine, expiresLine);

	string content = JSONHandler.createJsonNotReady();

	message += "HTTP/1.1 200 OK";
	message += CRLF;
	message += dateLine;
	message += CRLF;
	message += "Location: 127.0.0.1/eda_coin/PING";
	message += CRLF;
	message += "Cache-Control: max-age=30";
	message += CRLF;
	message += expiresLine;
	message += CRLF;
	message += "Content-Length: ";
	message += to_string(content.length());
	message += CRLF;
	message += "Content-Type: application/x-www-form-urlencoded";
	message += CRLF;
	message += CRLF;
	message += content;
	message += CRLF;

	return message;
}

string FULLNode::createServerReadyRsp()
{
	string message;
	char dateLine[100];
	char expiresLine[100];
	createDates(dateLine, expiresLine);
	string content = JSONHandler.createJsonReady(layout, blockChain);

	message += "HTTP/1.1 200 OK";
	message += CRLF;
	message += dateLine;
	message += CRLF;
	message += "Location: 127.0.0.1/eda_coin/PING";
	message += CRLF;
	message += "Cache-Control: max-age=30";
	message += CRLF;
	message += expiresLine;
	message += CRLF;
	message += "Content-Length: ";
	message += to_string(content.length());
	message += CRLF;
	message += "Content-Type: application/x-www-form-urlencoded";
	message += CRLF;
	message += CRLF;
	message += content;
	message += CRLF;

	return message;
}

string FULLNode::createServerErrRsp()
{
	string message;
	char dateLine[100];
	char expiresLine[100];
	createDates(dateLine, expiresLine);
	string content = JSONHandler.createJsonErr(); //Estari bueno pasarle un errorType

	message += "HTTP/1.1 404 Not Found";
	message += CRLF;
	message += dateLine;
	message += CRLF;
	message += "Cache-Control: public, max-age=30";
	message += CRLF;
	message += expiresLine;
	message += CRLF;
	message += "Content-Length: ";
	message += to_string(content.length());
	message += CRLF;
	message += "Content-Type: application/x-www-form-urlencoded";
	message += CRLF;
	message += CRLF;
	message += content;
	message += CRLF;

	return message;
}


string FULLNode::createServerHeader(string path, string id)
{
	string message;
	char dateLine[100];
	char expiresLine[100];
	createDates(dateLine, expiresLine);
	string content = JSONHandler.createJsonBlockHeader(blockChain, id); 

	message += "HTTP/1.1 200 OK";
	message += CRLF;
	message += dateLine;
	message += CRLF;
	message += "Location: 127.0.0.1" + path;
	message += CRLF;
	message += "Cache-Control: max-age=30";
	message += CRLF;
	message += expiresLine;
	message += CRLF;
	message += "Content-Length: ";
	message += to_string(content.length());
	message += CRLF;
	message += "Content-Type: application/x-www-form-urlencoded";
	message += CRLF;
	message += CRLF;
	message += content;
	message += CRLF;

	return message;
}

string FULLNode::createServerOkRsp(string path)
{
	string message;
	char dateLine[100];
	char expiresLine[100];
	createDates(dateLine, expiresLine);

	string content = JSONHandler.createJsonOk();

	message += "HTTP/1.1 200 OK";
	message += CRLF;
	message += dateLine;
	message += CRLF;
	message += "Location: 127.0.0.1" + path;
	message += CRLF;
	message += "Cache-Control: max-age=30";
	message += CRLF;
	message += expiresLine;
	message += CRLF;
	message += "Content-Length: ";
	message += to_string(content.length());
	message += CRLF;
	message += "Content-Type: application/x-www-form-urlencoded";
	message += CRLF;
	message += CRLF;
	message += content;
	message += CRLF;

	return message;
}


void FULLNode::createDates(char* c1, char* c2)
{
	//Fecha actual
	time_t currentTime = time(nullptr);
	struct tm t;
	struct tm* currTime = &t;
	gmtime_s(currTime, &currentTime);
	strftime(c1, 100, "Date: %a, %d %b %G %X GMT", currTime);

	//Fecha de expiracion
	struct tm t2 = t;
	struct tm* nextTime = &t2;
	if (nextTime->tm_sec >= 30) {
		if (nextTime->tm_min == 59) {
			nextTime->tm_hour++;
		}
		nextTime->tm_min = ((nextTime->tm_min) + 1) % 60;
	}
	nextTime->tm_sec = ((nextTime->tm_sec) + 30) % 60;
	strftime(c2, 100, "Expires: %a, %d %b %G %X GMT", nextTime);
}

/***********************************************************************************
		FLOODING / VERIFICATION
***********************************************************************************/
/*void FULLNode::checkForFilter(Block blck) 
{
	
}*/

bool FULLNode::checkForId(string id)
{
	bool ret = false;
	for (int i = 0; i < blockChain.size(); i++)
	{
		if (blockChain[i].getBlockID() == id)
		{
			ret = true;
		}
	}

	return ret;
}


void FULLNode::floodBlock(Block blck, NodeData sender) {
	for (int i = 0; i < neighbourhood.size(); i++) {
		if (sender.getID() != neighbourhood[i].getID())					//If neighbour is not the one who sent the block
			postBlock(i, blockChain.size());							//Send neighbour the block ()
	}
}
void FULLNode::floodTx(Transaction tx, NodeData sender) {
	for (int i = 0; i < neighbourhood.size(); i++) {
		if (sender.getID() != neighbourhood[i].getID())					//If neighbour is not the one who sent the tx
			postTransaction(i, tx);										//Send neighbour the tx
	}
}