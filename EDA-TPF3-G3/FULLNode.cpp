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
		//listen to port
		//if(received ping from another Full node)
			//nodeState = WAITING_LAYOUT;
			//break;
		//Pick random timeout
		//if(clock > timeout)
			//nodeState = COLLECTING_MEMBERS;
		break;
	case COLLECTING_MEMBERS:			//Look at me, I build the network now
		//pickup all nodes from the Manifest
		//for(all net members WHO HAVENT RESONPONDED)
			//nonBlocking ping
		//for(all net members)
			//if(doneSending)			//Si algun curl termino de mandar su ping, y recibio una respuesta
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
		break;
	case WAITING_LAYOUT:
		keepListening();
		//if (doneReceivingLayout)
			//readLayout(message);					//Read layout and add my neighbours
			//nodeState = NETWORK_CREATED;
		break;
	case SENDING_LAYOUT:
		//keepSending();
		//for(clients)
			//if(doneSending)
				//if(OkMessageReceived)
					//REMOVE NODE FROM WAITING LIST
					//DESTROY CURL
				//if(notOkMsg)
					//COPY CURL INFO
					//CREATE COPY OF CURL, AND ADD TO CLIENT LIST (to retry sending the layout message)
					//DESTROY FAILED CURL

		//if(client.empty())
			//nodeState = NETWORK_CREATED
		break;
	case NETWORK_CREATED:
		keepListening();
		keepSending();
		break;
	default:
		break;
	}
}