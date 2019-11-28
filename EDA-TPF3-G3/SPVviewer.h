#pragma once
#include "observer.h"

#include "SPVNode.h"

class SPVviewer :
	public Observer
{
public:
	SPVviewer();
	virtual void update(void*);
	void cycle(); 

private:
	void showNodeType();

	NodeData nodedata;
	string windowName;
	tipo_de_nodo type;
	unsigned long int money;
	NodeData headernodedata;
	NodeData filternodedata;
};

