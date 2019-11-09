#pragma once
#include "observer.h"

//Includes por la jerarquia principal (NET)
#include "EDAcoinsNet.h"	//MODEL
#include "NETviewer.h"		//VIEWER

//Includes por la jerarquia secundaria (Nodes)
#include "FULLNode.h"		//MODEL
#include "FULLcontroller.h"	//CONTROLLER
#include "FULLviewer.h"		//VIEWER

#include "SPVNode.h"		//MODEL
#include "SPVcontroller.h"	//CONTROLLER
#include "SPVviewer.h"		//VIEWER

//Ventanas de Warning
#include "WarningWindowHandler.h"

//Graficas	//LINKEAR!!!!!!!!!!!!!!!!!!!!!!
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "imgui.h"
#include "imgui_impl_allegro5.h"

class NETcontroller :
	public Observer
{
public:
	NETcontroller(EDAcoinsNet& model);
	~NETcontroller();
	virtual void update(void*);
	bool isRunning();
	void cycle();
private:
	EDAcoinsNet* netmodel;
	NETviewer* netviewer;
	bool running;
};

