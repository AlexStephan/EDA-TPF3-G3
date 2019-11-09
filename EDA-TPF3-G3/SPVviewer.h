#pragma once
#include "observer.h"

#include "SPVNode.h"

class SPVviewer :
	public Observer
{
public:
	virtual void update(void*);
	void cycle();
};

