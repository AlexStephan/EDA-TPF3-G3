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

//ImGuiHandler
#include "ImGuiHandler.h"

class NETcontroller :
	public Observer
{
public:
	NETcontroller(EDAcoinsNet* model);
	~NETcontroller();
	virtual void update(void*);
	bool isRunning();
	void cycle();
private:
	EDAcoinsNet* netmodel;
	NETviewer netviewer;
	bool running;
	int type;
	char IDbuf[50];
	int nodePort;
	int IParr[4];
	int currFilter;
	int currHeader;
	string nodeName;
	string FullNames;

	ImGuiHandler guiHandler;
	WarningWindowHandler warningHandler;

	//Para los nodos locales
	vector<FULLcontroller*> FcontrolList;
	vector<FULLviewer*> FviewList;
	vector<SPVcontroller*> ScontrolList;
	vector<SPVviewer*> SviewList;

	string myWindowName;

	//METODOS PRIVADOS:
	void drawControlWindow();
	void findFullNames();

};

