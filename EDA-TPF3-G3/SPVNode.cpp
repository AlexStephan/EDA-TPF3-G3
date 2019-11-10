#include "SPVNode.h"

SPVNode::SPVNode(NodeData ownData, NodeData filterNode, NodeData headerNode) {

}

SPVNode::~SPVNode() {

}

void SPVNode::cycle() {
	keepSending();
	keepListening();
}