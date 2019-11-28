#pragma once

//       \\\\\           \\\\\           \\\\\           \\\\\    
//      \\\\\\\__o      \\\\\\\__o      \\\\\\\__o      \\\\\\\__o
//______\\\\\\\'/_______\\\\\\\'/_______\\\\\\\'/_______\\\\\\\'/_
//toda la clase

#include "FULLNode.h"
class MINERNode :
	public FULLNode
{
public:
	MINERNode(Socket _socket);
	~MINERNode() {}
	virtual void cycle(); //Tarea para Ale: copiar pegar, y no olvidarse de llamar a miningActions
	errorType getWindowMessage();

private:
	void miningActions();

	Block miningBlock;
	vector<errorType> mensajesDeVentana;

	//TAREA PARA ALEJO
	bool miningBlockVerifyChallente();
};

