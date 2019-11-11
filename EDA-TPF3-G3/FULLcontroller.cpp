#include "FULLcontroller.h"
#include "imgui.h"
#include "imgui_stdlib.h"

#define CHILD_W	220
#define CHILD_H	330
#define BUTTON_S	100

void FULLcontroller::cycle()
{
	ImGui::Begin(windowID.c_str());
	ImGui::BeginChild("FULL CONTROL", ImVec2(CHILD_W, CHILD_H));
	switch (cstate) {
	case MENU:
		drawWindow();
		break;
	case MTX:
		drawMTX();
		break;
	case MBLOCK:
		drawMBlock();
		break;
	case ADDN:
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
		"ADD\nNEIGHBOUR"
	};

	for (int i = 0; i < 3; i++) {
		ImGui::PushID(i);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 0.9f, 0.9f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 1.0f, 1.0f));
		if (ImGui::Button(BUTTON_TEXT[i], ImVec2(BUTTON_S, BUTTON_S)))
			cstate = FULLcontrolState(MTX + i);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

}

void FULLcontroller::drawMBlock() {
	returnButton();
	ImGui::Text("\nMAKE BLOCK\n\n");

	if (ImGui::Button("MAKE BLOCK")) {
		warningHandler.check(fnode->makeBlock());
		cstate = MENU;
	}
}


void FULLcontroller::drawMTX() {
	returnButton();
	ImGui::Text("\nMAKE A TRANSACTION\n\n");

	for (int i = 0; i < currTX; i++) {
		if (drawVout(txData[i])) {
			currTX--;
			for (int j = i; j < currTX; j++)
				txData[j] = txData[j + 1];
			txData.resize(txData.size() - 1);
		}
	}
	
	if (txData[currTX - 1].publicId.size()) {
		Vout aux;
		currTX++;
		txData.push_back(aux);
	}

	ImGui::Text("Amount of Vouts: %d", currTX - 1);
	if (ImGui::Button("MAKE TRANSACTION")) {
		txData.pop_back();
		warningHandler.check(fnode->makeTX(txData));
		cstate = MENU;
	}

}

bool FULLcontroller::drawVout(Vout& aux){
	bool r = false;
	ImGui::InputText("Public ID", &aux.publicId);
	ImGui::SetNextItemWidth(50);
	ImGui::InputText("Monto", &auxstr);
	aux.amount = _atoi64(auxstr.c_str());
	
	if (ImGui::Button("Delete Vout"))
		r = true;

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
		cstate = MENU;
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
		cstate = MENU;
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


