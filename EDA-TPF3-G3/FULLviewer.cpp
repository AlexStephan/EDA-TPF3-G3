#include "FULLviewer.h"
#include "imgui.h"

#define NO_DATA "NO DATA",0,0,0,0,0

#define CHILD_W	300
#define CHILD_H	330

#define CCHILD_W (CHILD_W-40)
#define CCHILD_H 200

#define VCHILD_W (CCHILD_W-40)
#define VCHILD_H 100

FULLviewer::FULLviewer() :
	nodedata(NO_DATA), neigbours(), pendingTX(),
	windowName("NO_DATA")
{
}

void FULLviewer::update(void*n)
{
	FULLNode* node = (FULLNode*)n;

	nodedata = node->getData();
	neigbours = node->getNeighbours();
	pendingTX = node->getPendingTX();

	windowName = nodedata.getID();

}

void FULLviewer::printNodeData()
{
	if (ImGui::CollapsingHeader("Data")) {
		ImGui::Text("ID: %s", nodedata.getID().c_str());
		ImGui::Text("Port: %s", nodedata.getSocket().getPortString().c_str());
		ImGui::Text("IP: %s", nodedata.getSocket().getIPString().c_str());
	}
}

void FULLviewer::printNeighbours()
{
	if (ImGui::CollapsingHeader("Neighbours")) {
		if (neigbours->size() > 0) {
			for (int i = 0; i < neigbours->size(); i++) {
				ImGui::Text("ID: %s | Port: %s | IP: %s", (*neigbours)[i].getID().c_str(), (*neigbours)[i].getSocket().getPortString().c_str(), (*neigbours)[i].getSocket().getIPString().c_str());
			}
		}
		else {
			ImGui::Text("This node has no neighbours... and that's pretty strange, isn't it?");
		}
	}
}

void FULLviewer::printPendingTX()
{
	if (ImGui::CollapsingHeader("Transactions")) {

		if (pendingTX->size() > 0) {
			ImGui::Text(" ");
			ImGui::SameLine();
			ImGui::BeginChild("Txs", ImVec2(CCHILD_W, CCHILD_H));
			for (int i = 0; i < pendingTX->size(); i++) {
				string subWindowName = (*pendingTX)[i].txId + "##" + to_string(i);
				if (ImGui::CollapsingHeader(subWindowName.c_str())) {

					ImGui::Text("ID: %s", (*pendingTX)[i].txId.c_str());
					printVin(i);
					printVout(i);
				}
			}
			ImGui::EndChild();
		}
		else {
			ImGui::Text("There are no new Transactions");
		}
	}
}

void FULLviewer::printVin(int i)
{
	ImGui::Text("Number of incomes: %u", (*pendingTX)[i].nTxIn);
	ImGui::Text(" ");
	ImGui::SameLine();
	string vinwindow = "vins##" + to_string(i);
	ImGui::BeginChild(vinwindow.c_str(), ImVec2(VCHILD_W, VCHILD_H), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
	if (ImGui::CollapsingHeader("vIn")) {
		for (int j = 0; j < (*pendingTX)[i].vIn.size(); j++) {
			ImGui::Text("Block ID: %s\tTx ID: %s", (*pendingTX)[i].vIn[j].blockId.c_str(), (*pendingTX)[i].vIn[j].txId.c_str());
		}
	}
	ImGui::EndChild();
}

void FULLviewer::printVout(int i)
{
	ImGui::Text("Number of outcomes: %u", (*pendingTX)[i].nTxOut);
	ImGui::Text(" ");
	ImGui::SameLine();
	string voutwindow = "vout##" + to_string(i);
	ImGui::BeginChild(voutwindow.c_str(), ImVec2(VCHILD_W, VCHILD_H), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
	if (ImGui::CollapsingHeader("vOut")) {
		for (int j = 0; j < (*pendingTX)[i].vOut.size(); j++) {
			ImGui::Text("Public ID: %s\tTx Amount: %lu", (*pendingTX)[i].vOut[j].publicId.c_str(), (*pendingTX)[i].vOut[j].amount);
		}
	}
	ImGui::EndChild();
}
