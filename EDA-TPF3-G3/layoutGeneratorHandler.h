#pragma once

#include <vector>
#include "NodeData.h"
#include "layout.h"




class layoutGeneratorHandler
{
public:
	layoutGeneratorHandler();
	layoutGeneratorHandler(const NodeData& ownData, const vector<NodeData>& network);
	void changeData(const NodeData& ownData, const vector<NodeData>& network);
	bool generateLayout();
	Layout getLayout();
private:
	const NodeData* callingNode;
	const vector<NodeData>* network;

	
};

