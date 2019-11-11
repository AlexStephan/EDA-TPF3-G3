/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "SPVNode.h"

/*******************************************************************************
	CONSTRUTOR
 ******************************************************************************/
SPVNode::SPVNode(NodeData ownData, NodeData FilterNode, NodeData HeaderNode) : Node(ownData), filterNode(FilterNode), headerNode(HeaderNode) {
	
	//Create Genesis Server
	Server* genesisServer = new Server(ownData.getSocket().getPort());
	genesisServer->startConnection();			//Preguntar si esto funcaria
	servers.push_back(genesisServer);
}

/*******************************************************************************
	DESTRUCTOR
 ******************************************************************************/
SPVNode::~SPVNode() {
	cout << "Destroyed SPV node" << endl;

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
void SPVNode::cycle() {
	keepSending();
	keepListening();
}
/***********************************************************************************
	METHODS USED BY CONTROLLER
***********************************************************************************/
errorType SPVNode::makeTX(const vector<Vout>& receivers, const vector<Vin>& givers) {
	Transaction tx;
	Block aux;
	//BUILD TX
	tx.vIn = givers;
	tx.nTxIn = givers.size();
	tx.vOut = receivers;
	tx.nTxOut = receivers.size();
	tx.txId = aux.getTxId(tx);
	//POST TX TO A NEIGHBOUR, JUST ONE
	postTransaction(tx);
}

errorType SPVNode::postFilter()
{
	Client* client = new Client(filterNode);
	string id = JSONHandler.createJsonFilter(filterNode.getID());
	client->POST("/eda_coin/send_filter", id);
	errorType err = client->sendRequest();
	clients.push_back(client);
	notifyAllObservers();
	return err;
}

errorType SPVNode::changeFilterNode(NodeData FilterNode) { filterNode = FilterNode; }
errorType SPVNode::changeHeaderNode(NodeData HeaderNode) { headerNode = HeaderNode; }

/***********************************************************************************
	METHODS USED BY VIEWER
***********************************************************************************/
NodeData SPVNode::getData(){ return ownData; }
NodeData SPVNode::getFilterNodeData() { return filterNode; }
NodeData SPVNode::getHeaderNodeData() { return headerNode; }

/***********************************************************************************
	NETWORKING SH*T
***********************************************************************************/
void SPVNode::keepListening() {
	vector<vector<Server*>::iterator> deleteThis;
	vector<Server*> doneServers;
	servers.back()->listening();

	if ((*(servers.end() - 1))->getDoneListening()) {
		cout << "Latest Server picked up something!" << endl;
		Server* newServer = new Server(ownData.getSocket().getPort());
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
		switch ((*j)->getState()) {
		case MERKLE:														//SPV NODES ONLY CARE ABOUT RECEIVING MERKLEBLOCKS ON SERVER
			JSONHandler.saveMerkleBlock((*j)->getMessage(), mBlocks);
			//REQUEST CONFIRMATION OF VALIDATION
			getBlockHeader(mBlocks.back().blockId);							//SUPER DUDOSO LO QUE ACABO DE HACER, PERO ONDA, SUPER DUDOSO
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

void SPVNode::keepSending() {
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
		if ((*j)->getClientType() == GETClient) {
			//ANALIZE BLOCK HEADER RECEIVED
			//AND COMPARE TO LATEST MERKLE BLOCK RECEIVED IN mBlocks
			//POP BACK LATEST MERKLE BLOCK IN LIST
		}
		//And delete completed client
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
errorType SPVNode::getBlockHeader(string id)
{
	errorType err = { false,"" };
	Client* client = new Client(headerNode);
	string header = JSONHandler.createHeader(id);
	cout << "JSON:" << endl << header << endl;	//DEBUG
	client->GET("/eda_coin/get_block_header/", header);
	client->sendRequest();
	clients.push_back(client);
	notifyAllObservers();
	return err;
}

errorType SPVNode::postTransaction(Transaction tx)
{
	Client* client = new Client(headerNode);
	string tx_ = JSONHandler.createJsonTx(tx);
	cout << "JSON:" << endl << tx_ << endl;	//DEBUG
	client->POST("/eda_coin/send_tx", tx_);
	errorType err = client->sendRequest();
	clients.push_back(client);
	notifyAllObservers();
	return err;
}