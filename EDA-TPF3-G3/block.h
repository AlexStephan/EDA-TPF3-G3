#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <string>
#include <vector>
#include "Socket.h"

/*******************************************************************************
 * NAMESPACES
 ******************************************************************************/
using namespace std;

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 *****************************************************************************/
typedef std::string newIDstr;
typedef unsigned long int longN;
typedef vector<newIDstr> MerkleTree;

//       \\\\\           \\\\\           \\\\\           \\\\\    
//      \\\\\\\__o      \\\\\\\__o      \\\\\\\__o      \\\\\\\__o
//______\\\\\\\'/_______\\\\\\\'/_______\\\\\\\'/_______\\\\\\\'/_
/********
 * VIN	*
********/
struct Vin {
	string blockId;
	string txId;
	int nutxo;
	string signature;
};

/********
 * VOUT	*
********/
struct Vout {
	string publicId;
	unsigned long int amount;
};

/****************
 * TRANSACTION	*
****************/
struct Transaction {
	string txId;
	unsigned long int nTxIn;
	vector<Vin> vIn;
	unsigned long int nTxOut;
	vector<Vout> vOut;
};

/****************
 * MERKLE BLOCK	*
****************/
struct MerkleBlock {
	string blockId;
	vector<Transaction> tx;
	int txPos;
	vector<newIDstr> merklePath;
};

/****************
 *    FILTER   	*
****************/
struct Filter{
	string publicID;
	ip_t ip;
	unsigned int port;

	string getIPString()
	{
		return (to_string(ip.b1) + '.' + to_string(ip.b2) + '.' + to_string(ip.b3) + '.' + to_string(ip.b4));
	}

};
//DUMMY TX
//const Transaction dummyTX = { "DUMMY",2,{{"A","B"},{"C","D"}},3,{{"ALE",20},{"GIAN",30},{"OLI",40}} };



class Block {
public:
	/***********************************************************************************
		BLOCK RELATED METHODS
	***********************************************************************************/
	bool validateBlock(string block);
	void saveBlock(string blck);
	
	/***********************************************************************************
		GETTERS
	***********************************************************************************/
	const vector<Transaction>& getTransactions();
	const Transaction getTx(vector<Transaction>::iterator it);
	const Transaction getTx(unsigned int it);
	const unsigned long int getHeight() const;
	const unsigned long int getNonce();
	const unsigned long int getNTx();
	const long int getBlockPos(vector<Block>* BlockChain);
	const string getBlockID() const;
	const string getPreviousBlockID();
	const string getMerkleRoot();
	const string getTxId(Transaction tx);

	/***********************************************************************************
		MERKLE TREE & MERKLE PATH
	***********************************************************************************/
	vector<newIDstr> getMerkleTree();
	vector<string> getMerklePath(Transaction trx);
	newIDstr getRootFromPath(vector<newIDstr> path);

	/***********************************************************************************
		SETTERS
	***********************************************************************************/
	void addTx(Transaction _tx) { tx.push_back(_tx); };
	void setHeight(unsigned long int h) { height = h; };
	void setNonce(unsigned long int h) { nonce = h; };
	void setBlockId(string s) { blockId = s; };
	void setPrevBlockId(string s) { previousBlockId = s; };
	void setMerkleRoot(string s) { merkleRoot = s; };
	void setNTx(unsigned long int n) { nTx = n; };
	void setTransactions(vector <Transaction> txs) { tx = txs; };


private:
	/***********************************************************************************
		MERKLE TREE & MERKLE PATH
	***********************************************************************************/
	void fillLevel(int level, int* nearestPow, vector<newIDstr>::iterator it, vector<newIDstr>* tree);
	void fillMerklePath(vector<newIDstr>* path, vector<newIDstr>* tree, int j);

	/***********************************************************************************
		BLOCK STRUCTURE
	***********************************************************************************/
	vector<Transaction> tx;
	unsigned long int height;
	unsigned long int nonce;
	string blockId;
	string previousBlockId;
	string merkleRoot;
	vector<string> merkleTree;
	unsigned long int nTx;
};