#include "SPVcontroller.h"
#include "imgui.h"
#include "imgui_stdlib.h"

#define CHILD_W	220
#define CHILD_H	330
#define BUTTON_S	100
#define BUTTONS_PER_ROW 2

void SPVcontroller::cycle()
{
	ImGui::Begin(windowID.c_str());
	ImGui::BeginChild("SPV CONTROL", ImVec2(CHILD_W, CHILD_H));
	switch (cstate) {
	case MENU:
		drawWindow();
		break;
	case MTX:
		drawMTX();
		break;
	case PFILTER:
		drawPFilter();
		break;
	case CHANGEFN:
		drawChangeFN();
		break;
	case CHANGEHN:
		drawChangeHN();
		break;
	}
	ImGui::EndChild();
	ImGui::End();

	warningHandler.draw();
}

void SPVcontroller::drawWindow()
{
	const char* BUTTON_TEXT[4] = {
		"MAKE\nTX",
		"POST\nFILTER",
		"CHANGE\nFILTER NODE"
		"CHANGE\nHEADER NODE"
	};

	for (int i = 0; i < 4; i++) {
		if ((i % BUTTONS_PER_ROW) != 0)
			ImGui::SameLine(); 
		ImGui::PushID(i);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 0.9f, 0.9f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 1.0f, 1.0f));
		if (ImGui::Button(BUTTON_TEXT[i], ImVec2(BUTTON_S, BUTTON_S)))
			cstate = SPVcontrolState(MTX + i);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

}

void SPVcontroller::drawMTX() {
	returnButton();
	ImGui::Text("\nMAKE A TRANSACTION\n\n");

	for (int i = 0; i < currTX; i++) {
		if (drawV(txVin[i], txVout[i])) {
			currTX--;
			for (int j = i; j < currTX; j++) {
				txVin[j] = txVin[j + 1];
				txVout[j] = txVout[j + 1];
			}
			txVout.resize(txVout.size() - 1);
		}
	}

	if (txVout[currTX - 1].publicId.size() && txVout[currTX - 1].publicId.size()) {
		Vin vin;
		Vout vout;
		currTX++;
		txVin.push_back(vin);
		txVout.push_back(vout);
	}

	ImGui::Text("Amount of Vins and Vouts: %d", currTX - 1);
	if (ImGui::Button("MAKE TRANSACTION")) {
		txVin.pop_back();
		txVout.pop_back();
		warningHandler.check(snode->makeTX(txVout, txVin));
		cstate = MENU;
	}

}

bool SPVcontroller::drawV(Vin& vin, Vout& vout) {
	bool r = false;

	ImGui::Text("Vin");
	ImGui::InputText("Block ID", &vin.blockId);
	ImGui::InputText("Transaction ID", &vin.txId);

	ImGui::Text("Vout");
	ImGui::InputText("Public ID", &vout.publicId);
	ImGui::SetNextItemWidth(50);
	ImGui::InputText("Monto", &auxstr);
	vout.amount = _atoi64(auxstr.c_str());

	if (ImGui::Button("Delete##"))
		r = true;

	return r;
}

void SPVcontroller::drawPFilter() {
	returnButton();
	ImGui::Text("\nPOST FILTER\n\n");

	if (ImGui::Button("POST FILTER")) {
		warningHandler.check(snode->postFilter());
		cstate = MENU;
	}
}

void SPVcontroller::drawChangeFN() {
	returnButton();
	ImGui::Text("\nCHANGE FILTER NODE");

	ImGui::InputText("Filter Node ID", &newID);
	newPortSelect();
	newIpSelect();

	if (ImGui::Button("CHANGE FILTER NODE")) {
		warningHandler.check(snode->changeFilterNode(NodeData(newID, newPort, newIP[0], newIP[1], newIP[2], newIP[3])));
		cstate = MENU;
	}
}

void SPVcontroller::drawChangeHN() {
	returnButton();
	ImGui::Text("\nCHANGE HEADER NODE");

	ImGui::InputText("Header Node ID", &newID);
	newPortSelect();
	newIpSelect();

	if (ImGui::Button("CHANGE HEADER NODE")) {
		warningHandler.check(snode->changeHeaderNode(NodeData(newID, newPort, newIP[0], newIP[1], newIP[2], newIP[3])));
		cstate = MENU;
	}
}

void SPVcontroller::newPortSelect(){
	ImGui::Text("Neighbour Port:");
	ImGui::SetNextItemWidth(50);
	ImGui::DragInt("##Port", &newPort, 0.5);
}

void SPVcontroller::newIpSelect(){
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

void SPVcontroller::returnButton() {

	if (ImGui::Button("Return"))
		cstate = MENU;
}



