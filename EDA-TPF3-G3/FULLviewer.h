#pragma once
#include "observer.h"

#include "FULLNode.h"

class FULLviewer :
	public Observer
{
public:
	virtual void update(void*);
	void cycle();
};

