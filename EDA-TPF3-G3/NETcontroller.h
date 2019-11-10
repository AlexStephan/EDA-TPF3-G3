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

	ImGuiHandler guiHandler;
	WarningWindowHandler warningHandler;

	//Para los nodos locales
	vector<FULLcontroller*> FcontrolList;
	vector<FULLviewer*> FviewList;
	vector<SPVcontroller*> ScontrolList;
	vector<SPVviewer*> SviewList;

	string myWindowName;

	//METODOS PRIVADOS:
	void drawControlWindow(); //OLI
	/*NOTA de drawControlWindow:
	usar myWindowName como parametro de ImGui::Begin
	creo q solo es necesario el include "imgui.h" q ya coloque

	El NETcontroller accede (en su caracter de controller)
	a las siguientes funciones de EDAcoinsNET:

	errorType createFULLNode(NodeData newNode);
	errorType createSPVNode(NodeData newNode, NodeData FilterNode, NodeData HeaderNode);

	recordar siempre pasar el resultado de dichas funciones (errorType) a traves de
	warningHandler.check();

	La unica funcion de la ventana de controller es permitirle al usuario
	elegir q tipo de nodo crear, y luego, los parametros de entrada
	(NodeData propia, en caso de ser FULL, o propia + de los dos FULL vecinos, para los SPV)

	*/
};

