#include "NETcontroller.h"
#include "imgui.h"

#define WINDOW_NAME "EDA Coins Net"

#define CLEAN(vector)	for(int i=0;i<(vector).size();i++) delete (vector)[i]
#define CYCLE(vector)	for(int i=0;i<(vector).size();i++) (vector)[i]->cycle()

NETcontroller::NETcontroller(EDAcoinsNet* model) :
	netmodel(model), myWindowName(WINDOW_NAME),
	netviewer(WINDOW_NAME), running(true),
	guiHandler(), warningHandler(WINDOW_NAME),
	FcontrolList(), FviewList(),
	ScontrolList(), SviewList()
{
	netmodel->attach(netviewer);
}

NETcontroller::~NETcontroller(){
	CLEAN(FcontrolList);
	CLEAN(FviewList);
	CLEAN(ScontrolList);
	CLEAN(SviewList);
}

void NETcontroller::update(void*)
{
	size_t index = FcontrolList.size();
	if (netmodel->getFULLamount() > index) {
		FULLNode* newNode = netmodel->getFULLnode(index);

		FULLcontroller* newController = new FULLcontroller(newNode);
		FULLviewer* newViewer = new FULLviewer();

		newNode->attach(*newController);
		newNode->attach(*newViewer);

		FcontrolList.emplace_back(newController);
		FviewList.emplace_back(newViewer);

		newViewer->update(newNode);
	}

	index = ScontrolList.size();
	if (netmodel->getSPVamount() > index) {
		SPVNode* newNode = netmodel->getSPVnode(index);

		SPVcontroller* newController = new SPVcontroller(newNode);
		SPVviewer* newViewer = new SPVviewer();

		newNode->attach(*newController);
		newNode->attach(*newViewer);

		ScontrolList.emplace_back(newController);
		SviewList.emplace_back(newViewer);

		newViewer->update(newNode);
	}
}


bool NETcontroller::isRunning()
{
	return running;
}

void NETcontroller::cycle()
{
	guiHandler.start_frame(running);

	drawControlWindow();
	netviewer.cycle();

	CYCLE(FcontrolList);
	CYCLE(FviewList);
	CYCLE(ScontrolList);
	CYCLE(SviewList);

	warningHandler.draw();


	guiHandler.end_frame();
}


