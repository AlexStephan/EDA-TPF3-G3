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
	BlockChain* blockchain;
	TreeWindowHandler treeHandler;

	void drawWindow();	//	OLI
	/*

	1/3

	Info del FULL para imprimir
	-Data (con la funcion ya hecha printNodeData)
	-Neighbours (usar printNeighbours)
	-TX pendientes (printPendingTX)

	para la ultima, se usa una funcion llamada printTx, de printTXroutine, tambien es tu deber modificarla si algo de ahi no te gusta
	O sea, por lo pronto, es solo llamar a esas 3 funciones, salvo q no te guste el estilo de las ventanas (q es el mismo q en el TP anterior)

	2/3
	ahora, agregarle a drawWindow una llamada a la funcion printBlockList
	esta no la hice, asi q es tu mision hacerla.
	esto es lo q tengo en mente:
		en primer lugar, q tenga una "solapa", al estilo de las otras 3 funciones q mencione mas arriba
		al abrirla, q apareza una lista (por ID o height, o ambas, como:
			1) blablabla
			2) nyehehe
			...
		listando todos los bloques presentes en la blockchain. La idea es q se pueda seleccionar cualquier
		bloque, y al pulsarlo, simplemente tenes q llamar a
			treeHandler.createWindow(bloque en cuestion)
		y la magia del MVC se encarga del resto
		o sea, q sea como botones. no te molestes en preguntarte cuales bloques ya fueron pulsados o no
		indistintamente si ya estan abiertas o no las ventanas de los trees, vos llama a esa funcion, el handler se encarga del resto
	
	Ah, cierto, los bloques se encuentran (asumi ya cargados, aunque pueden ser 0) en
	"blockchain"

	Buena suerte. Cualquier cosa consultame.

	Proximamente, parte 3/3

	*/

	void printNodeData();
	void printNeighbours();
	void printPendingTX();	//OLI!! Fijarse en la funcion printTx para cambiar el comportamiento de esa parte

	void printBlockList();


//	void printVin(int i);	//DO NOT ERASE (YET)
//	void printVout(int i);
};

