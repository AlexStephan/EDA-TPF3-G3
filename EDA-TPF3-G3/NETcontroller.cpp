#include "NETcontroller.h"

#define WINDOW_NAME "EDA Coins Net"

#define CLEAN(vector)	for(int i=0;i<(vector).size();i++) delete (vector)[i]
#define CYCLE(vector)	for(int i=0;i<(vector).size();i++) (vector)[i]->cycle()

NETcontroller::NETcontroller(EDAcoinsNet& model) :
	netmodel(&model), myWindowName(WINDOW_NAME),
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


