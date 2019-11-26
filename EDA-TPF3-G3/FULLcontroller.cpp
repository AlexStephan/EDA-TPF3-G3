#include "FULLcontroller.h"
#include "imgui.h"
#include "imgui_stdlib.h"

#define CHILD_W	220
#define CHILD_H	330
#define BUTTON_S		100
#define BUTTONS_PER_ROW	2

void FULLcontroller::cycle()
{
	ImGui::Begin(windowID.c_str());
	ImGui::BeginChild("FULL CONTROL", ImVec2(CHILD_W, CHILD_H));
	switch (cstate) {
	case FULL_MENU:
		drawWindow();
		break;
	case FULL_MTX:
		drawMTX();
		break;
	case FULL_MBLOCK:
		drawMBlock();
		break;
	case FULL_ADDN:
		drawAddNode();
		break;
	}
	ImGui::EndChild();
	ImGui::End();

	warningHandler.draw();
}

void FULLcontroller::drawWindow()
{
	const char* BUTTON_TEXT[3] = {
		"MAKE\nTX",
		"MAKE\nBLOCK",
		"ADD NEIGHBOUR"
	};

	for (int i = 0; i < 3; i++) {
		if ((i % BUTTONS_PER_ROW) != 0)
			ImGui::SameLine();
		ImGui::PushID(i);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 0.9f, 0.9f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 1.0f, 1.0f));
		if (i < 2) {
			if (ImGui::Button(BUTTON_TEXT[i], ImVec2(BUTTON_S, BUTTON_S)))
				cstate = FULLcontrolState(FULL_MTX + i);
		}
		else {
			if (ImGui::Button(BUTTON_TEXT[i], ImVec2(2 * BUTTON_S + 8, BUTTON_S)))
				cstate = FULLcontrolState(FULL_MTX + i);
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

}

void FULLcontroller::drawMBlock() {
	returnButton();
	ImGui::Text("\nMAKE BLOCK\n\n");

	if (ImGui::Button("MAKE BLOCK")) {
		warningHandler.check(fnode->makeBlock());
		cstate = FULL_MENU;
	}
}


void FULLcontroller::drawMTX() {
	returnButton();
	ImGui::Text("\nMAKE A TRANSACTION\n\n");

	for (int i = 0; i < currTX; i++) {
		if (txVin.size() == 0 || txVout.size() == 0) {
			Vin VinAux;
			txVin.push_back(VinAux);
			Vout VoutAux;
			txVout.push_back(VoutAux);
			string aux;
			auxstr.push_back(aux);
		}
		if (drawV(txVin[i], txVout[i], i) && (txVin.size() != 1 && txVout.size() != 1)) {
			currTX--;
			for (int j = i; j < currTX; j++) {
				txVin[j] = txVin[j + 1];
				txVout[j] = txVout[j + 1];
				auxstr[j] = auxstr[j + 1];
			}
			txVin.resize(txVin.size() - 1);
			txVout.resize(txVout.size() - 1);
			auxstr.resize(auxstr.size() - 1);
		}
	}
	
	if (txVin[currTX - 1].blockId.size() && txVout[currTX - 1].publicId.size()) {
		Vin vin;
		Vout vout;
		string aux;
		currTX++;
		txVin.push_back(vin);
		txVout.push_back(vout);
		auxstr.push_back(aux);
	}

	ImGui::Text("Amount of Vins and Vouts: %d", currTX - 1);
	if (ImGui::Button("MAKE TRANSACTION")) {
		txVin.pop_back();
		txVout.pop_back();
		warningHandler.check(fnode->makeTX(txVout, txVin));
		cstate = FULL_MENU;
	}

}

bool FULLcontroller::drawV(Vin& vin, Vout& vout, int i){
	bool r = false;
	
	ImGui::Text("Vin");

	ImGui::InputText(("Block ID##" + to_string(i)).c_str(), &vin.blockId);
	ImGui::InputText(("Transaction ID##" + to_string(i)).c_str(), &vin.txId);

	ImGui::Text("Vout");
	ImGui::InputText(("Public ID##" + to_string(i)).c_str(), &vout.publicId);
	ImGui::SetNextItemWidth(50);
	ImGui::InputText(("Monto##" + to_string(i)).c_str(), &auxstr[i]);
	vout.amount = _atoi64(auxstr[i].c_str());
	
	if (ImGui::Button(("Delete##" + to_string(i)).c_str()))
		r = true;

	ImGui::NewLine();

	return r;
}


void FULLcontroller::drawAddNode() {
	returnButton();
	ImGui::Text("\nADD A NEIGHBOUR\n\n");
	ImGui::InputText("Neighbour ID", &newID);
	newPortSelect();
	newIpSelect();

	if (ImGui::Button("ADD NEIGHBOUR")) {
		warningHandler.check(fnode->addNeighbour(NodeData(newID, newPort, newIP[0], newIP[1], newIP[2], newIP[3])));
		cstate = FULL_MENU;
	}
}

void FULLcontroller::newPortSelect(){
	ImGui::Text("Neighbour Port:");
	ImGui::SetNextItemWidth(50);
	ImGui::DragInt("##Port", &newPort, 0.5);
}

void FULLcontroller::newIpSelect(){
	ImGui::Text("Neighbour IP:");
	ImGui::PushItemWidth(25);
	ImGui::DragInt(".##1", newIP, 0.5);
	ImGui::SameLine();
	ImGui::DragInt(".##2", newIP + 1, 0.5);
	ImGui::SameLine();
	ImGui::DragInt(".##3", newIP + 2, 0.5);
	ImGui::SameLine();
	ImGui::DragInt("##4", newIP + 3, 0.5);
	ImGui::PopItemWidth();
}


void FULLcontroller::returnButton() {

	if (ImGui::Button("Return"))
		cstate = FULL_MENU;
}

//void FULLcontroller::neighbourSelect()
//{
//	if (fnode->getNeighbours()->size()) {
//		int curr = 0;
//		ImGui::Text("Neighbour nodes -empty-");
//		ImGui::Combo("##Neighbour nodes -empty-", &curr, "\0");
//	}
//	else {
//		ImGui::Text("Neighbour nodes");
//		ImGui::Combo("##Neighbour nodes", &currNeighbour, findNeighbourNames());
//	}
//}
//
//const char* FULLcontroller::findNeighbourNames() {
//	string neighbourNames;
//
//	for (int i = 0; i < fnode->getNeighbours()->size(); i++) {
//		neighbourNames += fnode->getNeighbours()[currNeighbour]->getData().getID();
//		neighbourNames += '\0';
//	}
//	neighbourNames += '\0';
//
//	return neighbourNames.c_str();
//}

/*errorType makeTX(const vector<Vout>& receivers);
	errorType makeBlock(); //"MINAR"
	errorType addNeighbour(NodeData neighbour); //agrega fulls*/


