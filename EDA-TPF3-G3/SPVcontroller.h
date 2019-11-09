#pragma once
#include "observer.h"

#include "SPVNode.h"
#include "WarningWindowHandler.h"

class SPVcontroller :
	public Observer
{
public:
	SPVcontroller(SPVNode* model) : snode(model), windowID(model->getData().getID()),
		warningHandler(model->getData().getID()) {}
	virtual void update(void*) {

	}
	void cycle() {
		drawWindow();
		warningHandler.draw();
	}
private:
	void drawWindow(); //OLI

	SPVNode* snode;
	string windowID;
	WarningWindowHandler warningHandler;

	BlockChain Bchain;
};

