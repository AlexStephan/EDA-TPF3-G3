#include "TreeWindowHandler.h"

TreeWindowHandler::TreeWindowHandler() :
	windowList(), nodeId("NO_DATA")
{
}

void TreeWindowHandler::setId(const string& id)
{
	nodeId = id;
}

void TreeWindowHandler::createWindow(Block& block)
{
	auto it = windowList.find(block.getHeight()); //because iterators are a pain in the... neck
	if (it == windowList.end())
		windowList.emplace(block.getHeight(), (block, nodeId));
	else
		it->second.replaceBlock(block,nodeId);
}

void TreeWindowHandler::draw()
{
	if (windowList.empty() == false) {
		map<unsigned int, TreeWindow>::iterator closing, it; //auto no me salvo de esta
		bool closeWindow = false;

		for (it = windowList.begin(); it != windowList.end(); it++) {
			it->second.draw();
			if (it->second.isOpen() == false) {
				closeWindow = true;
				closing = it;
			}
		}

		if (closeWindow == true) {
			windowList.erase(closing);
		}

	}
}
