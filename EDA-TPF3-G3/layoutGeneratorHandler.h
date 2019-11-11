#pragma once

#include <vector>
#include <random>
#include "NodeData.h"
#include "layout.h"

#include "layoutNumericConection.h"



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
	Layout generatedLayout;
	minstd_rand0 random;

	vector<layoutNumericConection> numericLayout;

	void createNumericLayout();

	void generateConections();
	void makeConnection(size_t index, size_t& markedNodes);
	void generate1Node();
	void generate2Nodes();

	void conectSubGraphs();

	bool allNodesMarked(size_t& markedNodes);
	size_t getRandomNode();
	size_t getUnmarkedNode();
	void unmarkAll();

	
};

