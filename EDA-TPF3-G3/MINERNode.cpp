#include "MINERNode.h"

MINERNode::MINERNode(Socket _socket) :
	FULLNode(_socket,NODO_MINERO), miningBlock(),
	mensajesDeVentana()
{
	cryptohandler.setMiningBlock(&miningBlock);
	utxohandler.setMiningBlock(&miningBlock);
}



errorType MINERNode::getWindowMessage()
{
	errorType mensaje;
	if (mensajesDeVentana.size() == 0) {
		mensaje.error = false;
		mensaje.datos = "";
	}
	else {
		mensaje = mensajesDeVentana.back();
		mensajesDeVentana.pop_back();
	}
	return mensaje;
}

void MINERNode::miningActions()
{
	cryptohandler.tryNewNonce();
	if (miningBlockVerifyChallente()) {

	}
}
