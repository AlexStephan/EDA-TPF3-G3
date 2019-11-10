#include "EDAcoinsNet.h"

EDAcoinsNet::EDAcoinsNet() :
	FULLdata(), SPVdata(), FULLvector(), SPVvector()
{
}

EDAcoinsNet::~EDAcoinsNet()
{
	for (size_t i = 0; i < FULLvector.size(); i++)
		delete FULLvector[i];

	for (size_t i = 0; i < SPVvector.size(); i++)
		delete SPVvector[i];
}
errorType EDAcoinsNet::createFULLNode(NodeData newNode)
{
	errorType creationState;
	creationState.error = false;
	creationState.datos = "";

	if (existAlready(newNode) == false) {
		FULLNode* node = new FULLNode(newNode);
		FULLvector.emplace_back(node);
	}
	else {
		creationState.error = true;
		creationState.datos = "Failed to create FULL Node:\nSpecified socket or ID is already used";
	}
	notifyAllObservers();
	return creationState;
}
errorType EDAcoinsNet::createSPVNode(NodeData newNode, NodeData FilterNode, NodeData HeaderNode)
{
	errorType creationState;
	creationState.error = false;
	creationState.datos = "";

	if (existAlready(newNode) == false) {
		SPVNode* node = new SPVNode(newNode, FilterNode, HeaderNode);
		SPVvector.emplace_back(node);
	}
	else {
		creationState.error = true;
		creationState.datos = "Failed to create SPV Node:\nSpecified socket or ID is already used";
	}
	notifyAllObservers();
	return creationState;
}
const vector<NodeData>& EDAcoinsNet::getKnownFULLdata()
{
	return FULLdata;
}

const vector<NodeData>& EDAcoinsNet::getKnownSPVdata()
{
	return SPVdata;
}

size_t EDAcoinsNet::getFULLamount()
{
	return FULLvector.size();
}

size_t EDAcoinsNet::getSPVamount()
{
	return SPVvector.size();
}

FULLNode* EDAcoinsNet::getFULLnode(size_t pos)
{
	FULLNode* node = nullptr;
	if (pos < FULLvector.size())
		node = FULLvector[pos];
	return node;
}

SPVNode* EDAcoinsNet::getSPVnode(size_t pos)
{
	SPVNode* node = nullptr;
	if (pos < SPVvector.size())
		node = SPVvector[pos];
	return node;
}

bool EDAcoinsNet::existAlready(NodeData node)
{
	if ((existAlready(node.getID()) == false)
		&& (existAlready(node.getSocket()) == false))
		return false;
	else
		return true;
}

bool EDAcoinsNet::existAlready(string ID)
{
	bool existence = false;

	for (size_t i = 0;
		(existence == false) && (i < FULLdata.size());
		i++) {

		if (FULLdata[i].getID() == ID)
			existence = true;
	}

	for (size_t i = 0;
		(existence == false) && (i < SPVdata.size());
		i++) {

		if (SPVdata[i].getID() == ID)
			existence = true;
	}

	return existence;
}

bool EDAcoinsNet::existAlready(Socket socket)
{
	bool existence = false;

	for (size_t i = 0;
		(existence == false) && (i < FULLdata.size());
		i++) {

		if (FULLdata[i].getSocket() == socket)
			existence = true;
	}

	for (size_t i = 0;
		(existence == false) && (i < SPVdata.size());
		i++) {

		if (SPVdata[i].getSocket() == socket)
			existence = true;
	}

	return existence;
}
