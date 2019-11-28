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
		if (txVout.size() == 0) {
			Vout VoutAux;
			txVout.push_back(VoutAux);
			string aux;
			auxstr.push_back(aux);
		}
		if (drawV(txVout[i], i) && (txVout.size() != 1)) {
			currTX--;
			for (int j = i; j < currTX; j++) {
				txVout[j] = txVout[j + 1];
				auxstr[j] = auxstr[j + 1];
			}
			txVout.resize(txVout.size() - 1);
			auxstr.resize(auxstr.size() - 1);
		}
	}
	
	if (txVout[currTX - 1].publicId.size()) {
		Vout vout;
		string aux;
		currTX++;
		txVout.push_back(vout);
		auxstr.push_back(aux);
	}

	ImGui::Text("Amount of Vouts: %d", currTX - 1);
	ImGui::SetNextItemWidth(50);
	ImGui::InputText("Fee", &feeStr);

	if (ImGui::Button("MAKE TRANSACTION")) {
		errfee = false;
		if (isAllDigits(feeStr)) {
			unsigned long int fee = _atoi64(feeStr.c_str());
			txVout.pop_back();
			warningHandler.check(fnode->makeTX(txVout, fee));
			txVout.clear();
			auxstr.clear();
			feeStr.clear();
			currTX = 1;
			cstate = FULL_MENU;
		}
		else
			errfee = true;
	}

	if (errfee)
		ImGui::Text("Por favor ingrese un valor de fee valido");

}

bool FULLcontroller::drawV(Vout& vout, int i){
	bool r = false;
	
	ImGui::Text("Vout");
	ImGui::InputText(("Public ID##" + to_string(i)).c_str(), &vout.publicId);
	ImGui::SetNextItemWidth(50);
	ImGui::InputText(("Monto##" + to_string(i)).c_str(), &auxstr[i]);
	
	if (ImGui::Button(("Delete##" + to_string(i)).c_str())) {
		if (isAllDigits(auxstr[i])) {
			vout.amount = _atoi64(auxstr[i].c_str());
			r = true;
		}
	}

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

bool FULLcontroller::isAllDigits(string s) {
	bool r = true;
	if (!s.size()) 
		r = false;
	else{
		for (int i = 0; i < s.size(); i++) {
			if (s[i] < '0' || s[i] > '9') {
				r = false;
				break;
			}
		}
	}

	return r;
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


