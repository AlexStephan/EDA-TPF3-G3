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
typedef std::string newIDstr;	//Cambie de parecer. Q los IDs sean strings
typedef unsigned long int longN;
typedef vector<newIDstr> MerkleTree;

struct Vin {
	string blockId;
	string txId;
};
struct Vout {
	string publicId;
	unsigned long int amount;
};
struct Transaction {
	string txId;
	unsigned long int nTxIn;
	vector<Vin> vIn;
	unsigned long int nTxOut;
	vector<Vout> vOut;
};

struct MerkleBlock {
	string blockId;
	vector<Transaction> tx;
	int txPos;
	vector<newIDstr> merklePath;
};

//DUMMY TX
const Transaction dummyTX = { "DUMMY",2,{{"A","B"},{"C","D"}},3,{{"ALE",20},{"GIAN",30},{"OLI",40}} };



class Block {
public:
	//TPF2 Methods
	bool validateBlock(string block);
	void saveBlock(string blck);
	//Getters
	vector<string> getMerklePath(Transaction trx);
	vector<Transaction> getTransactions() { return tx; };
	Transaction getTx(vector<Transaction>::iterator it) { for (auto i = tx.begin(); i != tx.end(); i++) { if (i == it) return *i; } };
	Transaction getTx(unsigned int it) { if (it >= 0 && it < tx.size())	return tx[it]; };
	unsigned long int getHeight() { return height; };
	unsigned long int getNonce() { return nonce; };
	string getBlockID() { return blockId; };
	string getPreviousBlockID() { return previousBlockId; };
	string getMerkleRoot() { return merkleRoot; };
	unsigned long int getNTx() { return nTx; };
	long int getBlockPos(vector<Block>* BlockChain);
	vector<newIDstr> getMerkleTree();

	//Setters
	void addTx(Transaction _tx) { tx.push_back(_tx); };
	void setHeight(unsigned long int h) { height = h; };
	void setNonce(unsigned long int h) { nonce = h; };
	void setBlockId(string s) { blockId = s; };
	void setPrevBlockId(string s) { previousBlockId = s; };
	void setMerkleRoot(string s) { merkleRoot = s; };
	void setNTx(unsigned long int n) { nTx = n; };
private:

	void fillLevel(int level, int* nearestPow, vector<newIDstr>::iterator it, vector<newIDstr>* tree);
	void fillMerklePath(vector<newIDstr>* path, vector<newIDstr>* tree, int j);

	vector<Transaction> tx;
	unsigned long int height;
	unsigned long int nonce;
	string blockId;
	string previousBlockId;
	string merkleRoot;
	vector<string> merkleTree;
	unsigned long int nTx;
};