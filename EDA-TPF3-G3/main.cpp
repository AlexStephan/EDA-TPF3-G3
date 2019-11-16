#include "EDAcoinsNet.h"
#include "NETcontroller.h"

//Turbio
bool FULLNode::isLedaderNode = false;

int main(void) {
	srand(time(NULL));
	EDAcoinsNet network;
	NETcontroller controller(&network);

	network.attach(controller);

	while (controller.isRunning())
		controller.cycle();

	return 0;
}