#include "layoutGeneratorHandler.h"
#include <chrono>


layoutGeneratorHandler::layoutGeneratorHandler()
	: callingNode(nullptr), network(nullptr),
	generatedLayout(), numericLayout(),
	random(chrono::system_clock::now().time_since_epoch().count())
{
}

layoutGeneratorHandler::layoutGeneratorHandler(const NodeData& ownData, const vector<NodeData>& network)
	: callingNode(&ownData), network(&network),
	generatedLayout(), numericLayout(),
	random(chrono::system_clock::now().time_since_epoch().count())
{
}

void layoutGeneratorHandler::changeData(const NodeData& ownData, const vector<NodeData>& net)
{
	callingNode = &ownData;
	network = &net;
}

bool layoutGeneratorHandler::generateLayout()
{
	bool result = false;
	generatedLayout.clear();


	if ((callingNode != nullptr) &&
		network != nullptr) {

		createNumericLayout();
		generateConections();
		conectSubGraphs();

		//make layout from graph!!!

		result = true;
	}

	return result;
}

Layout layoutGeneratorHandler::getLayout()
{
	return generatedLayout;
}

void layoutGeneratorHandler::createNumericLayout()
{
	numericLayout.clear();
	size_t amount = 1 + network->size();

	for (int i = 0; i < amount; i++)
		numericLayout.emplace_back(i);
}

void layoutGeneratorHandler::generateConections()
{
	size_t markedNodes = 0;

	if (numericLayout.size() == 1)
		generate1Node();
	else if (numericLayout.size() == 2)
		generate2Nodes();
	else {
		for (size_t i = 0; i < numericLayout.size(); i++) {
			while (numericLayout[i].isMarked() == false)
				makeConnection(i,markedNodes);
		}
	}
}

void layoutGeneratorHandler::makeConnection(size_t index, size_t& markedNodes)
{
	size_t newConection;
	if (allNodesMarked(markedNodes))
		newConection = getRandomNode();
	else
		newConection = getUnmarkedNode();

	numericLayout[index].initialConection(numericLayout[newConection], markedNodes);
}

void layoutGeneratorHandler::generate1Node()
{
	//Do nothing (1 node with no connections... how sad)
}

void layoutGeneratorHandler::generate2Nodes()
{
	numericLayout[0].initialConection(numericLayout[1]);
	//that's all they can do!
}

void layoutGeneratorHandler::conectSubGraphs()
{
	unmarkAll();
	bool connectedGraph = false;


}

bool layoutGeneratorHandler::allNodesMarked(size_t& markedNodes)
{
	if (markedNodes >= numericLayout.size())
		return true;
	else
		return false;
}

size_t layoutGeneratorHandler::getRandomNode()
{
	return (random() % numericLayout.size());
}

size_t layoutGeneratorHandler::getUnmarkedNode()
{
	size_t rta = getRandomNode();
	while (numericLayout[rta].isMarked() == true)
		rta = (rta + 1) % numericLayout.size();
	return rta;
}

void layoutGeneratorHandler::unmarkAll()
{
	for (size_t i = 0; i < numericLayout.size(); i++)
		numericLayout[i].unmark();
}
