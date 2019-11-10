#pragma once
#include "observer.h"

#include "FULLNode.h"

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

	void printNodeData();
	void printNeighbours();
	void printPendingTX();
	void printVin(int i);
	void printVout(int i);
};

