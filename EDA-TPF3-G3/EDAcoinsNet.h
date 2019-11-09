#pragma once
#include "subject.h"

#include "FULLNode.h"
#include "SPVNode.h"

#include "NodeData.h"
#include "errorType.h"

#include <vector>
#include <string>
using namespace std;

class EDAcoinsNet :
	public Subject
{
public:
	EDAcoinsNet();
	~EDAcoinsNet();
	errorType createFULLNode(NodeData newNode);
	errorType createSPVNode(NodeData newNode, NodeData FilterNode, NodeData HeaderNode);
	const vector<NodeData>& getKnownFULLdata();
	const vector<NodeData>& getKnownSPVdata();
	
	size_t getFULLamount();
	size_t getSPVamount();
	FULLNode* getFULLnode(size_t pos);
	SPVNode* getSPVnode(size_t pos);

private:
	vector<NodeData> FULLdata; //listas de nodos full conocidos (del MANIFIESTO -todas- y las siguientes creadas -EN ESTA MAQUINA-)
	vector<NodeData> SPVdata;

	vector<FULLNode*> FULLvector;
	vector<SPVNode*> SPVvector;

	bool existAlready(NodeData node);
	bool existAlready(string ID);
	bool existAlready(Socket socket);
};

