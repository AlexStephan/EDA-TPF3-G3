#include "SPVviewer.h"
#include "imgui.h"

#define NO_DATA	"NO DATA",0,0,0,0,0

SPVviewer::SPVviewer() :
	nodedata(NO_DATA), headernodedata(NO_DATA),
	filternodedata(NO_DATA), windowName("NO_DATA")
{
}

void SPVviewer::update(void* n)
{
	SPVNode* node = (SPVNode*)n;

	nodedata = node->getData();
	headernodedata = node->getHeaderNodeData();
	filternodedata = node->getFilterNodeData();

	windowName = nodedata.getID();
}

void SPVviewer::cycle() {
	ImGui::Begin(windowName.c_str());

	ImGui::Text("SPV Node ID: %s", nodedata.getID());
	ImGui::Text("SPV Node Port: %d", nodedata.getSocket().getPort());
	ImGui::Text("SPV Node IP: %s", nodedata.getSocket().getIPString());
	ImGui::NewLine();
	if (ImGui::CollapsingHeader("Filter Node")) {
		ImGui::Text("Filter Node ID: %s", filternodedata.getID());
		ImGui::Text("Filter Node Port: %d", filternodedata.getSocket().getPort());
		ImGui::Text("Filter Node IP: %s", filternodedata.getSocket().getIPString());
	}
	if (ImGui::CollapsingHeader("Header Node")) {
		ImGui::Text("Header Node ID: %s", headernodedata.getID());
		ImGui::Text("Header Node Port: %d", headernodedata.getSocket().getPort());
		ImGui::Text("Header Node IP: %s", headernodedata.getSocket().getIPString());
	}

	ImGui::End();
}