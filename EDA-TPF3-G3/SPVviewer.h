#pragma once
#include "observer.h"

#include "SPVNode.h"

class SPVviewer :
	public Observer
{
public:
	SPVviewer();
	virtual void update(void*);
	void cycle(); //OLI
	//Dibuja: datos del nodo, y datos de los dos nodos FULL vecinos
	//(especificando cual es cual)
	//puede usar los campos q ya estan definidos (se actualizan en el update)
	//asi q no es necesario llamar a metodos de SPV... creo
	//Recordar: nombre de la ventana: windowName


private:
	NodeData nodedata;
	string windowName;
	NodeData headernodedata;
	NodeData filternodedata;
};

