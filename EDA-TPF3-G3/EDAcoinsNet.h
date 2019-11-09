#pragma once
#include "subject.h"

#include "FULLNode.h"
#include "SPVNode.h"

#include <vector>

using namespace std;

class EDAcoinsNet :
	public Subject
{
	EDAcoinsNet();
	~EDAcoinsNet();
	errorType createFULLNode();
	errorType createSPVNode();
	const vector<>&
};

