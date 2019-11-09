#pragma once
#include "observer.h"

#include "FULLNode.h"
#include "WarningWindowHandler.h"

class FULLcontroller :
	public Observer
{
public:
	FULLcontroller(FULLNode* model) : fnode(model), windowID(model->getData().getID()),
		warningHandler(model->getData().getID()){}
	virtual void update(void*) {} //no se me ocurre q necesite el controller q se actualice
	void cycle() {
		drawWindow();
		warningHandler.draw();
	}
private:
	void drawWindow(); //OLI
	//Dibuja la ventana con el nombre windowID, y llama a las siguientes funciones
	//del model, cuando corresponda:
	/*
	errorType makeTX(const vector<Vout>& receivers);
	errorType makeBlock(); //"MINAR"
	errorType addNeighbour(NodeData neighbour); //agrega fulls
	*/
	//Recordar hacer warningHandler.check() con los retornos de dichas funciones


	FULLNode* fnode;
	string windowID;
	WarningWindowHandler warningHandler;
};

