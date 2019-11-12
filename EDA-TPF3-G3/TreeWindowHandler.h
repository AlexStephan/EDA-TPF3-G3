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

	void createWindow(const Block& block); // °w° -Estaria bueno hacer const a los getters de Block (no lo q devuelve, sino al metodo en si) asi puedo manejarme con const Blocks!!!! 
	void draw();
private:
	string nodeId;
	map<unsigned int,TreeWindow> windowList;
};

