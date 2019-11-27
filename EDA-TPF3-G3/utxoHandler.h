#pragma once

#include "utxo.h"
#include "errorType.h"

class utxoHandler
{
public:
	utxoHandler(BlockChain* blockChain, vector<Transaction>* txs);

	//WIP:
	void initializeUtxo(); //ASUMO Q LA BLOCKCHAIN ES VALIDA

	longN balance(string publicKey);

	bool TxExistAlready(Transaction& tx);
	errorType validateTX(Transaction& tx); //ANTES COMPROBAR HASH Y FIRMA
	errorType insertTX(Transaction& tx);

	bool BlockExistAlready(Block& block);
	errorType validateBlock(Block& block); //ANTES COMPROBAR HASH, CHALLENGE Y PREVIOUS_ID
	errorType insertBlock(Block& block);

private:
	vector<utxo> utxoList;
	vector<utxo> processingTxList;

	BlockChain* blockChain;
	vector<Transaction>* txs;

	bool vinRefersToUtxo(Vin& vin,size_t index);
	bool findUtxo(const string& id,int nutxo, size_t indexInList);
	
	bool vinRefersToProcessing(Vin& vin, size_t index);
	bool findProcessing(const string& id, int nutxo, size_t indexInList);
		
};

