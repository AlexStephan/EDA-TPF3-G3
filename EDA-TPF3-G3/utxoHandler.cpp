#include "utxoHandler.h"

#include <iostream>

#define BONUS 50 //cuanto gana el minero por minar (ademas del resto de cada tx)

utxoHandler::utxoHandler(BlockChain* blockChain, vector<Transaction>* txs)
	: blockChain(blockChain), txs(txs), utxoList()
{
}

void utxoHandler::initializeUtxo()
{
	for (size_t blockIndex = 0; blockIndex < blockChain->size(); blockIndex++) { //por cada bloque

		Block& currBlock = (*blockChain)[blockIndex];

		for (size_t txIndex = 0; txIndex < currBlock.getNTx(); txIndex++) { //Por cada TX

			Transaction currTx = currBlock.getTx(txIndex);

			for (size_t vinIndex = 0; vinIndex < currTx.nTxIn; vinIndex++) { //por cada Vin ...
				eraseUtxo(currTx.vIn[vinIndex]);								//... elimina un utxo
			}
			for (size_t voutIndex = 0; voutIndex < currTx.nTxOut; voutIndex++) { //y por cada Vout...
				addUtxo(currBlock, currTx, voutIndex);								//... agrega un utxo
			}

		}

	}
}

longN utxoHandler::balance(string publicKey)
{
	longN money = 0;
	for (size_t i = 0; i < utxoList.size(); i++) {
		if (utxoList[i].ownerID == publicKey) {
			money += utxoList[i].amount;
		}
	}
	return money;
}

//se fija en el txid
bool utxoHandler::TxExistAlready(Transaction& tx)
{
	for (size_t i = 0; i < txs->size(); i++) {
		if ((*txs)[i].txId == tx.txId) {
			return true;
		}
	}
	return false;
}

errorType utxoHandler::validateTX(Transaction& tx)
{
	errorType rta;
	rta.datos = "";
	rta.error = false;
	size_t index = 0;
	vector<size_t> utxoIndex = {};
	

	//Verifica que todos los miembros de Vin referencien a una UTXO
	for (size_t i = 0;rta.error==false && i < tx.nTxIn; i++) {
		if (vinRefersToUtxo(tx.vIn[i],index) == false) {
			utxoIndex.clear();
			rta.error = true;
			rta.datos = "TX invalida:\nElemento de Vin no referencia a UTXO";
		}
		else {
			utxoIndex.push_back(index);
		}
	}

	/*{
	//AQUI CORROBORAR Q NO SE USE UNA UTXO 2 VECES!!!
	}*/

	//Verifica que no se exceda el dinero disponible de los Vins
	if (rta.error == false) {
		longN income = 0;
		longN outcome = 0;

		//Suma de los montos del vin
		while (utxoIndex.size() > 0) {
			income += utxoList[utxoIndex.back()].amount;
			utxoIndex.pop_back();
		}
		for (size_t i = 0; i < tx.nTxOut; i++) {
			outcome += tx.vOut[i].amount;
		}

		if (income < outcome) {
			rta.error = true;
			rta.datos = "TX invalida:\nEl dinero entregado a los Vout excede el disponible en los Vin";
		}

	}

	return rta;
}

errorType utxoHandler::insertTX(Transaction& tx)
{
	errorType err;
	err.error = false;
	err.datos = "";
	size_t index = 0;
	for (size_t i = 0; err.error==false&& i < tx.nTxIn; i++) {
		if (vinRefersToUtxo(tx.vIn[i], index)) {
			processingTxList.emplace_back(utxoList[index]);
			utxoList.erase(utxoList.begin() + index);
		}
		else {
			err.error = true;
			err.datos = "FATAL ERROR:\nSe intento ingresar una TX que no cumple con la prueba de validacion\nUno de nosotros 4 no esta haciendo su trabajo...";
		}
	}


	if (err.error == false) {
		txs->emplace_back(tx);
	}

	return err;
}

bool utxoHandler::BlockExistAlready(Block& block)
{
	if (blockChain->size() >= block.getHeight()) {
		return true;
	}
	return false;
}

errorType utxoHandler::validateBlock(Block& block)
{
	errorType err;
	err.datos = "";
	err.error = false;

	size_t aux = 0;

	vector<size_t> utxoIndex = {};
	vector<size_t> procIndex = {};


	//verifica el height adecuado
	if (block.getHeight() != 1 + blockChain->size()) {
		err.error = true;
		err.datos = "Bloque invalido:\nEl height del bloque no corresponde con el esperado";
	}

	//Verifica q en cada TX, los Vins refieran a UTXOs o a PROCESSINGTX
	for (size_t txIndex = 0; err.error == false && txIndex < block.getNTx(); txIndex++) {
		Transaction currTx = block.getTx[txIndex];
		for (size_t vinIndex = 0; err.error == false && vinIndex < currTx.nTxIn; vinIndex++) {
			if (vinRefersToProcessing(currTx.vIn[vinIndex], aux)) {
				procIndex.push_back(aux);
			}
			else if (vinRefersToUtxo(currTx.vIn[vinIndex], aux)) {
				utxoIndex.push_back(aux);
			}
			else {
				err.error = true;
				err.datos = "Bloque invalido:\nUn miembro de Vin refiere a una TX no presente en la lista de UTXO ni en la lista de Processing TX";
			}
		}
	}

	longN income = 0;
	longN outcome = 0;

	if (err.error == false) {
		while (procIndex.size() > 0) {
			income += processingTxList[procIndex.back()].amount;
			procIndex.pop_back();
		}
		while (utxoIndex.size() > 0) {
			income += utxoList[utxoIndex.back()].amount;
			utxoIndex.pop_back();
		}

		for (size_t txIndex = 0; txIndex < block.getNTx(); txIndex++) {
			Transaction currTx = block.getTx(txIndex);
			for (size_t voutIndex = 0; voutIndex < currTx.nTxOut; voutIndex++) {
				outcome += currTx.vOut[voutIndex].amount;
			}
		}

		if (income + BONUS != outcome) {
			err.error = true;
			err.datos = "Bloque invalido:\nEl dinero proveniente de los Vins (+ 50) no coincide con el destinado a los Vouts";
		}
	}

	return err;
}

errorType utxoHandler::insertBlock(Block& block)
{
	errorType err;
	err.error = false;
	err.datos = "";

	size_t aux = 0;

	for (size_t txIndex = 0;
		err.error==false && txIndex < block.getNTx();
		txIndex++) {

		Transaction currTx = block.getTx(txIndex);

		for (size_t vinIndex = 0;
			err.error == false && vinIndex < currTx.nTxIn;
			vinIndex++) {

			if (vinRefersToProcessing(currTx.vIn[vinIndex], aux)) {
				processingTxList.erase(processingTxList.begin() + aux);
			}
			else if (vinRefersToUtxo(currTx.vIn[vinIndex], aux)) {
				utxoList.erase(utxoList.begin() + aux);
			}
			else {
				err.error = true;
				err.datos = "FATAL ERROR:\nSe intento ingresar un Bloque con un TX sin respaldo en utxo o processing tx\nSon las 2:30 AM de la mañana, y mi computadora parece tener mas ganas de dormir que yo";
			}

		}
	}

	for (size_t txIndex = 0;
		err.error == false && txIndex < block.getNTx();
		txIndex++) {

		Transaction currTx = block.getTx(txIndex);

		for (size_t voutIndex = 0;
			voutIndex < currTx.nTxOut;
			voutIndex++){
			
			addUtxo(block, currTx, voutIndex);
		}
	}

	if (err.error == false)
		blockChain->emplace_back(block);

	return err;
}

bool utxoHandler::vinRefersToUtxo(Vin& vin,size_t index)
{
	return findUtxo(vin.txId, vin.nutxo, index);
}

bool utxoHandler::findUtxo(const string& id,int nutxo, size_t indexInList)
{
	for (size_t i = 0; i < utxoList.size(); i++) {
		if (utxoList[i].txID == id && utxoList[i].nutxo == nutxo) {
			indexInList = i;
			return true;
		}
	}
	return false;
}

bool utxoHandler::vinRefersToProcessing(Vin& vin, size_t index)
{
	return findProcessing(vin.txId, vin.nutxo, index);
}

bool utxoHandler::findProcessing(const string& id, int nutxo, size_t indexInList)
{
	for (size_t i = 0; i < processingTxList.size(); i++) {
		if (processingTxList[i].txID == id && processingTxList[i].nutxo == nutxo) {
			indexInList = i;
			return true;
		}
	}
	return false;
}

void utxoHandler::addUtxo(Block& block, Transaction& tx,size_t voutIndex)
{
	utxo newUtxo;
	newUtxo.blockID = block.getBlockID();
	newUtxo.blockHeight = block.getHeight();
	newUtxo.txID = tx.txId;
	newUtxo.nutxo = voutIndex + 1;
	newUtxo.ownerID = tx.vOut[voutIndex].publicId;
	newUtxo.amount = tx.vOut[voutIndex].amount;

	utxoList.emplace_back(newUtxo);
}

void utxoHandler::eraseUtxo(Vin& vin)
{
	size_t index;
	if (vinRefersToUtxo(vin, index)) {
		utxoList.erase(utxoList.begin() + index);
	}
	else {
		std::cout << endl<<endl
			<<"***********************************************" <<endl
			<<"*          PROBLEMAS:                         *" <<endl
			<<"*                                             *" << endl
			<<"* El nodo recibio un blockChain no valido     *" << endl
			<<"* (los Vins no corresponden con utxos)        *" << endl
			<<"***********************************************" << endl
			<< endl << endl;
	}
}
