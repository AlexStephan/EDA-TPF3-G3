#include "FULLNode.h"

FULLNode::FULLNode(NodeData ownData) {
	nodeState = IDLE;
	saveBlockChain(blockChain, "BlockChain.json");
	//Timing sh*t
	clock = chrono::system_clock::now();
	int timing = rand() % 1000 + 1;
	chrono::duration<int, milli> dur(timing);
	timeout = dur;
}

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
			vector<NodeData> nodes;
			JSONHandler.getNodesInLayout("manifest.json", ownData, nodes);
			for (int i = 0; i < nodes.size(); i++) {
				postPing(nodes[i].getSocket());					//Ping each node in layout who isn't me (just a bit, rest of sending is done in COLLECTING_MEMBERS)
			}
		}
		break;
	case COLLECTING_MEMBERS:									//Look at me, I build the network now
		//pickup all nodes from the Manifest
		//for(all net members WHO HAVENT RESONPONDED)
			//nonBlocking ping
		//for(all net members)
			//if(doneSending)									//Si algun curl termino de mandar su ping, y recibio una respuesta
				//if(response == NetworkNotReady)
					//Add node to net
					//DESTROY CURL
					//REMOVE RESPONDING NODE FROM PING LIST
				//If(respones == NetworkReady)
					//makeLayout(network)
					//Add neighbour(el nodo que me respondio)
					//nodeState = SENDING_LAYOUTS
					//DESTROY CURL
					//REMOVE RESPONDING NODE FROM PING LIST
					//createClientsForLayout();
				//else
					//DESTROY CURL
					//CONFIGURE NEW CURL TO RESEND MESSAGE
		//DESTROY USELESS SH*T (CURL, MESSAGES, ETC)
		//if(allNodesFromManifestRespondedNEWTORK_NOT_READY)
			//nodeState = SENDING_LAYOUT;
			//createClientsForLayout();
			//vector<NodeData> nodes;
			//JSONHandler.getNodesInLayout("manifest.json", ownData, nodes);
			//for (int i = 0; i < nodes.size(); i++) {
				//postLayout(nodes[i].getSocket());					//Send Layout to every node in the manifest who isnt me (rest of msg is sent in SENDING_LAYOUT)
			//}
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
			Client* curr = clients[i];
			curr->sendRequest();
			if(curr->getRunning == 0)
				if (curr->getResponse() == HTTP_OK) {
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