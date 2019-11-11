#pragma once
#include "observer.h"

#include "FULLNode.h"
#include "WarningWindowHandler.h"

#define MAX_TX_ID	280
enum FULLcontrolState { MENU, MTX, MBLOCK, ADDN };

class FULLcontroller : public Observer
{
public:
	FULLcontroller(FULLNode* model) : fnode(model), windowID(model->getData().getID()),
		warningHandler(model->getData().getID()) {
		currTX = 1;
		cstate = MENU;
	}
	virtual void update(void*) {} //no se me ocurre q necesite el controller q se actualice
	void cycle();

private:
	void drawWindow();
	void drawMBlock();
	void drawMTX();
	void drawAddNode();
	bool drawV(Vin&, Vout&);
	void newPortSelect();
	void newIpSelect();
	void returnButton();

	//void neighbourSelect();
	//const char* findNeighbourNames();
	//Dibuja la ventana con el nombre windowID, y llama a las siguientes funciones
	//del model, cuando corresponda:
	/*
	errorType makeTX(const vector<Vout>& receivers);
	errorType makeBlock(); //"MINAR"
	errorType addNeighbour(NodeData neighbour); //agrega fulls
	*/
	//Recordar hacer warningHandler.check() con los retornos de dichas funciones

	int newPort;
	int newIP[4];
	string newID;
	int cstate;
	int currTX;
	string auxstr;
	//int currNeighbour;
	vector<Vout> txVout;
	vector<Vin> txVin;
	FULLNode* fnode;
	string windowID;
	WarningWindowHandler warningHandler;
};

