#pragma once
#include "observer.h"

#include "FULLNode.h"
#include "TreeWindowHandler.h"

class FULLviewer :
	public Observer
{
public:
	FULLviewer();
	virtual void update(void*);
	void cycle();

private:
	NodeData nodedata;
	string windowName;
	const vector<NodeData>* neigbours;
	const vector<Transaction>* pendingTX;
	TreeWindowHandler treeHandler;

	void drawWindow();	//OLI (descripcion WIP)

	void printNodeData();
	void printNeighbours();
	void printPendingTX();	//OLI!! Fijarse en la funcion printTx para cambiar el comportamiento de esa parte


//	void printVin(int i);	//DO NOT ERASE (YET)
//	void printVout(int i);
};

