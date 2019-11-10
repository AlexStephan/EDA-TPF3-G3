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
	virtual void update(void*) {} //tampoco se q le interese al controller
	void cycle() {
		drawWindow();
		warningHandler.draw();
	}
private:
	void drawWindow(); //OLI
	//Mismas indicaciones q drawWindow de FULLcontroller, pero en este caso accede a:
	/*
	errorType makeTX(const vector<Vout>& receivers);
	errorType postFilter();
	errorType changeFilterNode(NodeData FilterNode);
	errorType changeHeaderNode(NodeData HeaderNode);
	*/

	SPVNode* snode;
	string windowID;
	WarningWindowHandler warningHandler;
};

