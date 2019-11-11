/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "FULLNode.h"

/*******************************************************************************
	CONSTRUTOR
 ******************************************************************************/
FULLNode::FULLNode(NodeData ownData)
	: Node(ownData)
{
#include "layoutGeneratorHandler.h"

FULLNode::FULLNode(NodeData ownData) {
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
				postPing(nodesInManifest[i].getSocket());		//Ping each node in manifest who isn't me (just a bit, rest of sending is done in COLLECTING_MEMBERS)
			}
		}
		break;
	case COLLECTING_MEMBERS:									//Look at me, I build the network now
		int gotReady = -1;
		for (int i = 0; i < clients.size(); i++) {
			Client* curr = clients[i];
			curr->sendRequest();
			if (curr->getRunning == 0)
				if (curr->getResponse() == MSG_NETWORK_READY) {
					nodeState = SENDING_LAYOUT;
					gotReady = i;
					break;
				}
				else if (curr->getResponse() == MSG_NETWORK_NOT_READY) {
					network.push_back(curr->getRecepientNodeData());
					delete clients[i];								//Destroy client
					clients.erase(clients.begin() + i);				//Remove client from list
				}
				else{												//If none of the responses is received
					postPing(clients[i]->getSocket());				//Post ping again
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
			makeLayout(ownData, network, layout);
			layoutMsg = JSONHandler.createLayout(layout);
			if (gotReady != -1) {
				addNeighbour(clients[gotReady]->getRecepientNodeData());
			}
			for (int i = 0; i < clients.size(); i++) {
				delete clients[i];								//Destroy client
				clients.erase(clients.begin() + i);				//Remove client from list
			}
			clients.empty();
			for (int i = 0; i < network.size(); i++)
				postLayout(clients[i]->getSocket());				//Start posting Layout to all nodes in network
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
			if (servers.back()->getState() == GOT_LAYOUT) {					//If layout was correctly received
				JSONHandler.readLayout(servers.back()->getMessage(), ownData.getSocket(), neighbourhood);		//Read layout, and add my neighbours
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
				if (clients[i]->getResponse() == HTTP_OK) {
					delete clients[i];								//Destroy client
					clients.erase(clients.begin() + i);				//Remove client from list
				}
				else {
					postLayout(clients[i]->getSocket());			//Post layout again
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
errorType FULLNode::makeTX(const vector<Vout>& receivers) {
	Transaction tx;
	for (int i = 0; i < neighbourhood.size(); i++)
		postTransaction(i, tx);
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
		postBlock(i, blockChain.size());
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
	//Handle finished servers
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


errorType FULLNode::postLayout(Socket socket);



errorType FULLNode::postPing(Socket socket)
{
	Client* client = new Client(neighbourhood[neighbourPos]);
	client->POST("/eda_coin/send_block", blck);
	errorType err = client->sendRequest();
	clients.push_back(client);

	notifyAllObservers();
	return err;
}