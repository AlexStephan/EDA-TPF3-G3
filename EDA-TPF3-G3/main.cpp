#include "EDAcoinsNet.h"
#include "NETcontroller.h"

int main(void) {
	EDAcoinsNet network;
	NETcontroller controller(network);

	network.attach(controller);

	while (controller.isRunning())
		controller.cycle();

	return 0;
}