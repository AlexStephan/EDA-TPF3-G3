#pragma once
#include "observer.h"

#include "SPVNode.h"

class SPVcontroller :
	public Observer
{
public:
	SPVcontroller(SPVNode* model) : snode(model) {}
	virtual void update(void*);
	void cycle();
private:
	SPVNode* snode;
};

