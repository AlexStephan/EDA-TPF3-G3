#include "utxoHandler.h"

utxoHandler::utxoHandler(BlockChain* blockChain, vector<Transaction>* txs)
	: blockChain(blockChain), txs(txs), utxoList()
{
}

void utxoHandler::initializeUtxo()
{
	for (size_t i = 0; i < blockChain->size(); i++) {

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

bool utxoHandler::findUtxo(const string& id, size_t indexInList)
{
	for (size_t i = 0; i < utxoList.size(); i++) {
		if (utxoList[i].txID == id) {
			indexInList = i;
			return true;
		}
	}
	return false;
}
