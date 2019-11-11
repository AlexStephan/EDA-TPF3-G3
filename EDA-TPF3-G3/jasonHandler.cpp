
#include "jsonHandler.h"
#include "json.hpp"
#include <fstream>

/*******************************************************************************
 * NAMESPACES
 ******************************************************************************/
using namespace std;
using json = nlohmann::json;


/***********************************************************************************
	SAVING METHODS
***********************************************************************************/
void jsonHandler::saveBlockChain(BlockChain& blockchain, string path)
{
	std::ifstream i(path.c_str()); //Se puede cambiar, no se como recibo el JSON;
	json j;
	i >> j;

	for (auto& blocks : j)
	{
		//Block 
		Block block;

		auto height = blocks["height"];
		block.setHeight(height);

		auto nonce = blocks["nonce"];
		block.setNonce(nonce);

		auto blockId = blocks["blockid"];
		block.setBlockId(blockId.get<string>());

		auto prevBlockId = blocks["previousblockid"];
		block.setPrevBlockId(prevBlockId.get<string>());

		auto root = blocks["merkleroot"];
		block.setMerkleRoot(root.get<string>());

		auto nTx = blocks["nTx"];
		block.setNTx(nTx);

		//Transactions
		auto arrayTrans = blocks["tx"];
		for (auto& trans : arrayTrans)
		{
			Transaction auxTrans;

			auto txId = trans["txid"];
			auxTrans.txId = txId.get<string>();

			auto nTxIn = trans["nTxin"];
			auxTrans.nTxIn = nTxIn;

			auto vIn = trans["vin"];
			for (auto& elsi : vIn)
			{
				Vin auxVin;

				auto tBlockId = elsi["blockid"];
				auxVin.blockId = tBlockId.get<string>();

				auto tTxId = elsi["txid"];
				auxVin.txId = tTxId.get<string>();

				auxTrans.vIn.push_back(auxVin);
			}

			auto nTxOut = trans["nTxout"];
			auxTrans.nTxOut = nTxOut;

			auto vOut = trans["vout"];
			for (auto& elso : vOut)
			{
				Vout auxVout;

				auto publicId = elso["publicid"];
				auxVout.publicId = publicId.get<string>();

				auto amount = elso["amount"];
				auxVout.amount = amount;

				auxTrans.vOut.push_back(auxVout);
			}

			block.addTx(auxTrans);
		}

		blockchain.push_back(block);
	}
}


void jsonHandler::saveTx(string _trans, vector<Transaction>& txs)
{
	json trans;
	Transaction tx;
	trans = json::parse(_trans);
	auto txId = trans["txid"];
	tx.txId = txId.get<string>();
	auto nTxIn = trans["nTxin"];
	tx.nTxIn = nTxIn;
	auto vIn = trans["vin"];
	for (auto& elsi : vIn) {
		Vin auxVin;
		auto tBlockId = elsi["blockid"];
		auxVin.blockId = tBlockId.get<string>();
		auto tTxId = elsi["txid"];
		auxVin.txId = tTxId.get<string>();
		tx.vIn.push_back(auxVin);
	}
	auto nTxOut = trans["nTxout"];
	tx.nTxOut = nTxOut;
	auto vOut = trans["vout"];
	for (auto& elso : vOut) {
		Vout auxVout;
		auto publicId = elso["publicid"];
		auxVout.publicId = publicId.get<string>();
		auto amount = elso["amount"];
		auxVout.amount = amount;
		tx.vOut.push_back(auxVout);
	}
	txs.push_back(tx);
}

void jsonHandler::saveMerkleBlock(string _merkleBlock, vector<MerkleBlock>& mrkl)
{
	MerkleBlock mBlock;
	json merkleBlock = json::parse(_merkleBlock);

	auto blockId = merkleBlock["blockid"];
	mBlock.blockId = blockId.get<string>();

	auto mPath = merkleBlock["merklePath"];
	for (auto& elsi : mPath) {
		auto Id = elsi["Id"];
		mBlock.merklePath.push_back(Id.get<string>());
	}
	//Transactions
	auto arrayTrans = merkleBlock["tx"];
	for (auto& trans : arrayTrans)
	{
		Transaction auxTrans;

		auto txId = trans["txid"];
		auxTrans.txId = txId.get<string>();

		auto nTxIn = trans["nTxin"];
		auxTrans.nTxIn = nTxIn;

		auto vIn = trans["vin"];
		for (auto& elsi : vIn)
		{
			Vin auxVin;

			auto tBlockId = elsi["blockid"];
			auxVin.blockId = tBlockId.get<string>();

			auto tTxId = elsi["txid"];
			auxVin.txId = tTxId.get<string>();

			auxTrans.vIn.push_back(auxVin);
		}

		auto nTxOut = trans["nTxout"];
		auxTrans.nTxOut = nTxOut;

		auto vOut = trans["vout"];
		for (auto& elso : vOut)
		{
			Vout auxVout;

			auto publicId = elso["publicid"];
			auxVout.publicId = publicId.get<string>();

			auto amount = elso["amount"];
			auxVout.amount = amount;

			auxTrans.vOut.push_back(auxVout);
		}

		mBlock.tx.push_back(auxTrans);
	}

	auto txPos = merkleBlock["txPos"];
	mBlock.txPos = txPos;


	mrkl.push_back(mBlock);
}

/***********************************************************************************
	JSONS's CREATION
***********************************************************************************/
string jsonHandler::createJsonBlock(Block block)
{
	json blck;
	auto tx = json::array();
	for (int i = 0; i < block.getNTx(); i++)
	{
		tx += json::parse(createJsonTx(block.getTransactions()[i]));
	}
	blck["tx"] = tx;
	blck["height"] = block.getHeight();
	blck["nonce"] = block.getNonce();
	blck["blockid"] = block.getBlockID();
	blck["previousblockid"] = block.getPreviousBlockID();
	blck["merkleroot"] = block.getMerkleRoot();
	blck["nTx"] = block.getNTx();

	return blck.dump();
}


string jsonHandler::createJsonTx(Transaction trans)
{
	json tx;
	tx["txid"] = trans.txId;

	tx["nTxin"] = trans.nTxIn;
	auto vin = json::array();
	for (auto i = 0; i < trans.nTxIn; i++)
	{
		vin.push_back(json::object({ {"txid",trans.vIn[i].txId}, {"blockid", trans.vIn[i].blockId} }));
	}
	tx["vin"] = vin;

	tx["nTxout"] = trans.nTxOut;
	auto vout = json::array();
	for (auto i = 0; i < trans.nTxOut; i++)
	{
		vout.push_back(json::object({ {"amount",trans.vOut[i].amount}, {"publicid", trans.vOut[i].publicId} }));
	}
	tx["vout"] = vout;

	return tx.dump();
}

string jsonHandler::createJsonMerkle(Block block)
{
	json merkle;
	merkle["blockid"] = block.getBlockID();
	merkle["tx"][0] = json::parse(createJsonTx(block.getTransactions()[0]));
	merkle["txPos"] = 4;
	json mpath = json::array();
	MerkleTree path = block.getMerklePath(block.getTransactions()[0]);
	for (int i = 0; i < path.size(); i++)
	{
		mpath.push_back(json::object({ { "Id",path[i] } }));
	}
	merkle["merklePath"] = mpath;

	return merkle.dump();
}

string jsonHandler::createJsonFilter(string id)
{
	json filter;
	filter["Id"] = id;

	return filter.dump();
}
string jsonHandler::createHeader(string id)
{
	string head = "Header:'block_id':" + id;
	return head;
}


/***********************************************************************************
	JSONS's VALIDATION
***********************************************************************************/
errorType jsonHandler::validateBlock(string blck)
{
	errorType err = { true, "Wrong amount of fields/Wrong format" };

	try
	{
		json block = json::parse(blck);

		//Block
		if (block.size() == 7) //Si son 7 elementos
		{
			block.at("height");
			block.at("nonce");
			block.at("blockid");	//Se fija que sean los correspondientes
			block.at("previousblockid");
			block.at("merkleroot");
			int ntx = block.at("nTx");
			block.at("height");
			block.at("tx");

			//Transactions
			auto arrayTrans = block["tx"];
			for (auto& trans : arrayTrans)	//Parsea todas las transacciones
			{
				if (arrayTrans.size() == ntx && trans.size() == 5)	//Si son 5 elementos
				{
					trans.at("txid");
					int txin = trans.at("nTxin");
					trans.at("vin");	//Se fija que sean los correctos
					int txout = trans.at("nTxout");
					trans.at("vout");

					auto vIn = trans["vin"];
					auto vOut = trans["vout"];
					if (vIn.size() == txin && vOut.size() == txout)
					{
						for (auto& elsi : vIn)
						{
							elsi.at("blockid");
							elsi.at("txid");
						}

						for (auto& elso : vOut)
						{
							elso.at("publicid");
							elso.at("amount");
						}

						err = { false, "OK Block" };
					}
				}
			}
		}
	}

	catch (std::exception& e)
	{
		err = { true, "Invalid fields" };
	}

	return err;
}

errorType jsonHandler::validateTx(string tx)
{
	errorType err = { true, "Wrong amount of fields/Wrong format" };

	try
	{
		json trans = json::parse(tx);

		if (trans.size() == 5)	//Si son 5 elementos
		{
			trans.at("txid");
			int txin = trans.at("nTxin");
			trans.at("vin");	//Se fija que sean los correctos
			int txout = trans.at("nTxout");
			trans.at("vout");

			auto vIn = trans["vin"];
			auto vOut = trans["vout"];
			if (vIn.size() == txin && vOut.size() == txout)
			{
				for (auto& elsi : vIn)
				{
					elsi.at("blockid");
					elsi.at("txid");
				}

				for (auto& elso : vOut)
				{
					elso.at("publicid");
					elso.at("amount");
				}

				err = { false, "OK Transaction" };
			}
		}
	}

	catch (std::exception& e)
	{
		err = { true, "Invalid fields" };
	}

	return err;
}

errorType jsonHandler::validateFilter(string filter)
{
	errorType err = { true, "Missing field Id/Wrong format" };

	try
	{
		json fltr = json::parse(filter);
		fltr.at("Id");
		err = { false, "OK Filter" };
	}

	catch (std::exception& e)
	{
		err = { true, "Missing field Id/Wrong format" };
	}

	return err;
}


/***********************************************************************************
	LAYOUT CONTROLLERS
***********************************************************************************/
string jsonHandler::createJsonLayout(Layout& layout)
{
	json lays = json::object();
	for (int i = 0; i < layout.size(); i++)
	{
		lays["nodes"][i] = layout[i].ownData.getID();
		auto kcyo = json::object();
		for (int j = 0; j < layout[i].myNeighbours.size(); j++)
		{
			kcyo += {"tarjet" + to_string(j + 1), layout[i].myNeighbours[j].getID()};
		}
		lays["edges"][i] = kcyo;
	}

	return lays.dump();
}

void jsonHandler::readLayout(string layout, NodeData mySocket, vector <NodeData>& neighbourhood)
{
	json lay = json::parse(layout);
	auto nodes = lay["nodes"];
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].get<string>() == mySocket.getID())
		{
			auto arr = lay["edges"][i];
			for (auto& trjt : arr)
			{
				NodeData n(trjt.get<string>());
				neighbourhood.push_back(n);
			}
		}
	}

}


void jsonHandler::getNodesInLayout(string path, NodeData ownData, vector<NodeData>& nodes)
{
	ifstream i(path.c_str());
}