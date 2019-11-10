#include "SPVviewer.h"

#define NO_DATA	"NO DATA",0,0,0,0,0

SPVviewer::SPVviewer() :
	nodedata(NO_DATA), headernodedata(NO_DATA),
	filternodedata(NO_DATA), windowName("NO_DATA")
{
}

void SPVviewer::update(void* n)
{
	SPVNode* node = (SPVNode*)n;

	nodedata = node->getData();
	headernodedata = node->getHeaderNodeData();
	filternodedata = node->getFilterNodeData();

	windowName = nodedata.getID();
}
