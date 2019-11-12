#pragma once
#include "observer.h"

#include "SPVNode.h"
#include "WarningWindowHandler.h"

enum SPVcontrolState { SPV_MENU, SPV_MTX, SPV_PFILTER, SPV_CHANGEFN, SPV_CHANGEHN };

class SPVcontroller :
	public Observer
{
public:
	SPVcontroller(SPVNode* model) : snode(model), windowID(model->getData().getID()),
		warningHandler(model->getData().getID()) {
		cstate = SPV_MENU;
		currTX = 0;
	}
	virtual void update(void*) {} //tampoco se q le interese al controller
	void cycle();

private:
	void drawWindow(); 
	void drawMTX();
	bool drawV(Vin&, Vout&);
	void drawPFilter();
	void drawChangeFN();
	void drawChangeHN();
	void newPortSelect();
	void newIpSelect();
	void returnButton();


	//Mismas indicaciones q drawWindow de FULLcontroller, pero en este caso accede a:
	/*
	errorType makeTX(const vector<Vout>& receivers);
	errorType postFilter();
	errorType changeFilterNode(NodeData FilterNode);
	errorType changeHeaderNode(NodeData HeaderNode);
	*/
	int cstate;
	int currTX;
	string auxstr;
	vector<Vin> txVin;
	vector<Vout> txVout;
	string newID;
	int newPort;
	int newIP[4];
	SPVNode* snode;
	string windowID;
	WarningWindowHandler warningHandler;
};

