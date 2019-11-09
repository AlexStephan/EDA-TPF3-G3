#pragma once
#include "Node.h"

#include "manifestHandler.h"
#include "blockChain.h"

class FULLNode :
	public Node
{
public:
	FULLNode(NodeData ownData) : Node(ownData) {}
	virtual void cycle();

	//Metodos accedidos desde el controller
	errorType makeTX(const vector<Vout>& receivers);
	errorType makeBlock(); //"MINAR"
	errorType addNeighbour(NodeData neighbour); //agrega fulls

};

