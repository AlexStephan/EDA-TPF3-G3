#pragma once
#include "observer.h"

#include "FULLNode.h"

class FULLcontroller :
	public Observer
{
public:
	FULLcontroller(FULLNode* model) : fnode(model) {}
	virtual void update(void*);
	void cycle();
private:
	FULLNode* fnode;
};

