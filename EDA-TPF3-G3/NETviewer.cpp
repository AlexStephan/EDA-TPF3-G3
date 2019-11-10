#include "NETviewer.h"

#include "imgui.h"

NETviewer::NETviewer(const string& id) :
	myWindowName(id), listofFULL(), listofSPV()
{
}

void NETviewer::update(void* m)
{
	EDAcoinsNet* model = (EDAcoinsNet*)m;
	listofFULL = model->getKnownFULLdata();
	listofSPV = model->getKnownSPVdata();
}
