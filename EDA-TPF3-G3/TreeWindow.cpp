#include "TreeWindow.h"
#include "imgui.h"

TreeWindow::TreeWindow(const Block& b, const string& nodeId)
	: windowName(), open(true), block(b),
	validRoot(false), tree()
{
	windowName = "Block: " + block.getBlockID() + "##" + nodeId + "_" + to_string(block.getHeight());
	tree = block.getMerkleTree();
	validateRoot();
}

void TreeWindow::replaceBlock(const Block& b, const string& nodeId)
{
	block = b;
	windowName = block.getBlockID() + "##" + nodeId + "_" + to_string(block.getHeight());
	tree = block.getMerkleTree();
	validateRoot();
}

void TreeWindow::draw()
{
	ImGui::Begin(windowName.c_str(), &open);

	if (validRoot == false) {
		ImGui::Text("PRECAUCION! El merkle Root generado por Block (en el Tree) no coincide con el campo Merkle Root del mismo\n :<");
	}

	if (ImGui::TreeNode("Data")) {
		ImGui::Text("HEIGHT: %lu", block.getHeight());
		ImGui::Text("ID: %s", block.getBlockID().c_str());
		ImGui::Text("PREVIOUS BLOCK ID: %s", block.getPreviousBlockID().c_str());
		ImGui::Text("AMOUNT OF TX: %lu", block.getNTx());
		ImGui::Text("NONCE: %lu", block.getNonce());
		ImGui::Text("MERKLE ROOT: %s", block.getMerkleRoot());
		ImGui::TreePop();
	}

	drawTree();

	ImGui::End();
}

bool TreeWindow::isOpen()
{
	return open;
}

void TreeWindow::validateRoot(void)
{
	if (block.getMerkleRoot() == tree.back()) {
		validRoot = true;
	}
	else {
		validRoot = false;
	}
}
