#pragma once
#include "observer.h"

#include "FULLNode.h"
#include "TreeWindowHandler.h"

class FULLviewer :
	public Observer
{
public:
	FULLviewer();
	virtual void update(void*);
	void cycle();

private:
	NodeData nodedata;
	string windowName;
	const vector<NodeData>* neigbours;
	const vector<Transaction>* pendingTX;
	TreeWindowHandler treeHandler;

	void drawWindow();	//	OLI
	/*
	Info del FULL para imprimir
	-Data (con la funcion ya hecha printNodeData)
	-Neighbours (usar printNeighbours)
	-TX pendientes (printPendingTX)

	para la ultima, se usa una funcion llamada printTx, de printTXroutine, tambien es tu deber modificarla si algo de ahi no te gusta
	O sea, por lo pronto, es solo llamar a esas 3 funciones, salvo q no te guste el estilo de las ventanas (q es el mismo q en el TP anterior)

	pronto mas cosas
	
	*/

	void printNodeData();
	void printNeighbours();
	void printPendingTX();	//OLI!! Fijarse en la funcion printTx para cambiar el comportamiento de esa parte


//	void printVin(int i);	//DO NOT ERASE (YET)
//	void printVout(int i);
};

