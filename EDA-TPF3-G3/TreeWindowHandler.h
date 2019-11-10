#pragma once

#include "TreeWindow.h"
#include "blockChain.h"
#include <map>
#include <string>

class TreeWindowHandler
{
public:
	TreeWindowHandler();
	void setId(const string& id);

	void createWindow(const Block& block);
	void draw();
private:
	string nodeId;
	map<unsigned int,TreeWindow> windowList;
};

