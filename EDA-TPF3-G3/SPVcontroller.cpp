#include "SPVcontroller.h"
#include "imgui.h"
#include "imgui_stdlib.h"

#define CHILD_W	220
#define CHILD_H	330
#define BUTTON_S	100
#define BUTTONS_PER_ROW 2

void SPVcontroller::update(void* m)
{
	errorType newMessage;
	while ((newMessage = snode->getVerificationError()).error == true) {
		warningHandler.check(newMessage);
	}
}

void SPVcontroller::cycle()
{
	ImGui::Begin(windowID.c_str());
	ImGui::BeginChild("SPV CONTROL", ImVec2(CHILD_W, CHILD_H));
	switch (cstate) {
	case SPV_MENU:
		drawWindow();
		break;
	case SPV_MTX:
		drawMTX();
		break;
	case SPV_PFILTER:
		drawPFilter();
		break;
	case SPV_CHANGEFN:
		drawChangeFN();
		break;
	case SPV_CHANGEHN:
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
		"CHANGE\nFILTER NODE",
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
			cstate = SPVcontrolState(SPV_MTX + i);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

}

void SPVcontroller::drawMTX() {
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
		warningHandler.check(snode->makeTX(txVout, txVin));
		txVin.clear();
		txVout.clear();
		auxstr.clear();
		currTX = 1;
		cstate = SPV_MENU;
	}

}

bool SPVcontroller::drawV(Vin& vin, Vout& vout, int i) {
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

void SPVcontroller::drawPFilter() {
	returnButton();
	ImGui::Text("\nPOST FILTER\n\n");

	if (ImGui::Button("POST FILTER")) {
		warningHandler.check(snode->postFilter());
		cstate = SPV_MENU;
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
		cstate = SPV_MENU;
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
		cstate = SPV_MENU;
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
		cstate = SPV_MENU;
}



