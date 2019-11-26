#pragma once

#include "utxo.h"
#include "errorType.h"

class utxoHandler
{
public:
	utxoHandler(BlockChain* blockChain, vector<Transaction>* txs);
	void initializeUtxo(); //ASUMO Q LA BLOCKCHAIN ES VALIDA
	longN balance(string publicKey);

	bool TxExistAlready(Transaction& tx);
	errorType validateTX(Transaction& tx); //ANTES COMPROBAR HASH Y FIRMA
	void insertTX(Transaction& tx);

	bool BLockExistAlready(Block& block);
	errorType validateBlock(Block& block); //ANTES COMPROBAR HASH, CHALLENGE Y PREVIOUS_ID
	void insertBlock(Block& block);

private:
	vector<utxo> utxoList;

	BlockChain* blockChain;
	vector<Transaction>* txs;


	bool findUtxo(const string& id, size_t indexInList);
		
};

