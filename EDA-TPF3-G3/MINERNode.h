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
	~MINERNode();
	virtual void cycle();

	errorType getWindowMessage();

};

