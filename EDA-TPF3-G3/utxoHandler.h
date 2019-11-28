#pragma once

#include "utxo.h"
#include "errorType.h"
#include "typeEnums.h"
#include "cryptoHandler.h"

class utxoHandler
{
public:
	utxoHandler(tipo_de_nodo tipo,BlockChain* blockChain, vector<Transaction>* txs);

	longN balance(string publicKey);
	bool createTX(string myPublicKey, const vector<Vout>& receivers, Transaction& tx, longN fee);

	//SOLO FULL Y MINERO
	void initializeUtxo(); //ASUMO Q LA BLOCKCHAIN ES VALIDA


	bool TxExistAlready(Transaction& tx);
	errorType validateTX(Transaction& tx); //ANTES COMPROBAR HASH Y FIRMA
	errorType insertTX(Transaction& tx);

	bool BlockExistAlready(Block& block);
	errorType validateBlock(Block& block); //ANTES COMPROBAR HASH, CHALLENGE Y PREVIOUS_ID
	errorType insertBlock(Block& block);


	string getOwner(Vin& vin);


	//MINERO
	void setMiningBlock(Block* miningBlock);
	void startNewMiningBlock(string myPublicId, cryptoHandler& cryptohandler);

	//SPV
	void processHeader(MerkleBlock& merkle, unsigned long int height);
	

private:
	tipo_de_nodo tipo;
	bool checkMiner();
	Block* miningBlock;

	vector<utxo> utxoList;
	vector<utxo> processingTxList;

	BlockChain* blockChain;
	vector<Transaction>* txs;

	bool vinRefersToUtxo(Vin& vin,size_t& index);
	bool findUtxo(const string& id,int nutxo, size_t& indexInList);
	
	bool vinRefersToProcessing(Vin& vin, size_t& index);
	bool findProcessing(const string& id, int nutxo, size_t& indexInList);

	void addUtxo(Block& block, Transaction& tx,size_t voutIndex);
	void eraseUtxo(Vin& vin);
	Vin utxo2vin(size_t index);


	bool foundInBlockChain(Vin& vin,Vout& answer);
	bool foundInTXs(Transaction& tx, size_t& index);

	longN getMoneyFromVin(Vin& vin);
	longN getDifference(vector<Transaction>& enterTX);
};

