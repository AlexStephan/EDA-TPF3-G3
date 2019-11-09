#pragma once
#include "observer.h"
class NETviewer :
	public Observer
{
public:
	NETviewer(const string& id)
	: myWindowName(id){}
	void cycle();
	virtual void update(void*);
private:
	string myWindowName;
};

