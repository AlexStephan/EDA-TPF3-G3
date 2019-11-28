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

		//METERSE EL BLOQUE EN LA BLOCKCHAIN
		//FLODEAR EL BLOQUE
		errorType mensaje;
		mensaje.error = true;
		mensaje.datos = "ENHORABUENA!!!\nEste minero logro minar";
		mensajesDeVentana.emplace_back(mensaje);
		utxohandler.startNewMiningBlock();

	}
}
