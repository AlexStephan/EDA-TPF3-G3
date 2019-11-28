#include "SPVviewer.h"
#include "imgui.h"
#include "printTXroutine.h"

#define NOT_DATA	"NO DATA",0,0,0,0,0

SPVviewer::SPVviewer() :
	nodedata(NOT_DATA), headernodedata(NOT_DATA),
	filternodedata(NOT_DATA), windowName("NO_DATA")
{
}

void SPVviewer::update(void* n)
{
	SPVNode* node = (SPVNode*)n;

	nodedata = node->getData();
	headernodedata = node->
		NodeData();
	filternodedata = node->getFilterNodeData();

	windowName = nodedata.getID();
}

void SPVviewer::cycle() {
	ImGui::Begin(windowName.c_str());
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + CCHILD_W, ImGui::GetCursorPosY() - CHILD_H));
	ImGui::BeginChild("SPV VIEW", ImVec2(CHILD_W, CHILD_H));

	ImGui::Text("SPV Node ID: %s", nodedata.getID().c_str());
	ImGui::Text("SPV Node Port: %d", nodedata.getSocket().getPort());
	ImGui::Text("SPV Node IP: %s", nodedata.getSocket().getIPString().c_str());
	ImGui::NewLine();
	if (ImGui::CollapsingHeader("Filter Node")) {
		ImGui::Text("Filter Node ID: %s", filternodedata.getID().c_str());
		ImGui::Text("Filter Node Port: %d", filternodedata.getSocket().getPort());
		ImGui::Text("Filter Node IP: %s", filternodedata.getSocket().getIPString().c_str());
	}
	if (ImGui::CollapsingHeader("Header Node")) {
		ImGui::Text("Header Node ID: %s", headernodedata.getID().c_str());
		ImGui::Text("Header Node Port: %d", headernodedata.getSocket().getPort());
		ImGui::Text("Header Node IP: %s", headernodedata.getSocket().getIPString().c_str());
	}

	ImGui::EndChild();
	ImGui::End();
}