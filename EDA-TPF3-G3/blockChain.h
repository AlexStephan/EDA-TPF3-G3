#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <string>
#include <vector>

using namespace std;

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 *****************************************************************************/
struct Vin{
	string blockId;
	string txId;
};
struct Vout{
	string publicId;
	unsigned long int amount;
};
struct Transaction{
	string txId;
	unsigned long int nTxIn;
	vector<Vin> vIn;
	unsigned long int nTxOut;
	vector<Vout> vOut;
};


typedef std::string newIDstr;
typedef unsigned long int longN;
typedef vector<newIDstr> MerkleTree;

class Block {
public:
	vector<Transaction> getTransactions() { return tx; };
	Transaction getTx(vector<Transaction>::iterator it) { for (auto i = tx.begin(); i != tx.end(); i++) { if (i == it) return *i; } };
	Transaction getTx(unsigned int it) { if (it >= 0 && it < tx.size())	return tx[it]; };
	unsigned long int getHeight() { return height; };
	unsigned long int getNonce() { return nonce; };
	string getBlockID() { return blockId; };
	string getPreviousBlockID() { return previousBlockId; };
	string getMerkleRoot() { return merkleRoot; };
	unsigned long int getNTx() { return nTx; };
	

private:
	vector<Transaction> tx;
	unsigned long int height;
	unsigned long int nonce;
	string blockId;
	string previousBlockId;
	string merkleRoot;
	unsigned long int nTx;
};

typedef vector<Block> BlockChain;


//DUMMY TX
const Transaction dummyTX = { "DUMMY",2,{{"A","B"},{"C","D"}},3,{{"ALE",20},{"GIAN",30},{"OLI",40}} };
