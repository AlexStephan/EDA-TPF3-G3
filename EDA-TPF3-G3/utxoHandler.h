#pragma once

#include "utxo.h"

class utxoHandler
{
public:
	utxoHandler(BlockChain* blockChain, vector<Transaction>* txs);
	void initializeUtxo(); //ASUMO Q LA BLOCKCHAIN ES VALIDA
	longN balance(string publicKey);



private:
	vector<utxo> utxoList;

	BlockChain* blockChain;
	vector<Transaction>* txs;

	bool findUtxo(const string& id, size_t indexInList);
		
};

