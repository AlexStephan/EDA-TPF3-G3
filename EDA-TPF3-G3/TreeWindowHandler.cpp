#include "TreeWindowHandler.h"

TreeWindowHandler::TreeWindowHandler() :
	windowList(), nodeId("NO_DATA")
{
}

void TreeWindowHandler::setId(const string& id)
{
	nodeId = id;
}
