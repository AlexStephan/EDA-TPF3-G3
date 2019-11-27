#include "cryptoHandler.h"
#include "cryptoFunctions.h"

#include <cstring>

cryptoHandler::cryptoHandler()
{
	myprivateKey = generatePrivKey();
	myprivateKey.MakePublicKey(mypublicKey);

	byte auxBuf[HEADER_CHARS + PUBLIC_KEY_CHARS];
	ArraySink sink(auxBuf, HEADER_CHARS + PUBLIC_KEY_CHARS);
	mypublicKey.Save(sink);

	memcpy(header, auxBuf, HEADER_CHARS);
	
}

string cryptoHandler::getMyPrivateKey()
{
	vector<byte> b = privateKeyToByte(myprivateKey);
	return byteToString(b);
}

string cryptoHandler::getMyPublicKey()
{
	vector<byte> b = publicKeyToByte(mypublicKey);
	return byteToString(b);
}

void cryptoHandler::signAllVinsInTx(Transaction& tx)
{
	string lastPartOfMessage = concatenateVout(tx);

	for (size_t i = 0; i < tx.vIn.size(); i++) {
		Vin& currVin = tx.vIn[i];
		string message = "";
		message += currVin.blockId;
		message += currVin.txId;
		message += to_string(currVin.nutxo);
		message += lastPartOfMessage;

		string signature = signMessage(message);

		currVin.signature = signature;
	}
}

void cryptoHandler::hashTx(Transaction& tx)
{	
	tx.txId = makeHashFromTx(tx);
}

bool cryptoHandler::verifyTXHash(Transaction& tx)
{
	return (tx.txId == makeHashFromTx(tx));
}

bool cryptoHandler::verifyTXSign(Transaction& tx)
{
	bool rta = true;

	string lastPartOfMessage = concatenateVout(tx);

	for (size_t i = 0; rta == true && i < tx.vIn.size(); i++) {
		Vin& currVin = tx.vIn[i];
		string message = "";
		message += currVin.blockId;
		message += currVin.txId;
		message += to_string(currVin.nutxo);
		message += lastPartOfMessage;

		string signature = signMessage(message);

		rta = (currVin.signature == signature);
	}

	return rta;
}

string cryptoHandler::signMessage(string& message)
{
	vector<byte> b = getSignature(myprivateKey,message);
	return byteToString(b);
}

bool cryptoHandler::isSignValid(string& message, string& pubKey, string& sign)
{
	return verifySignatureString(header,pubKey,message,sign);
}

string cryptoHandler::hashMessage(string& message)
{
	return hashMessage(message);
}

bool cryptoHandler::isHashValid(string& message, string& hash)
{
	return (hash == hashMessage(message));
}

string cryptoHandler::makeHashFromTx(Transaction& tx)
{
	string message = "";

	message += to_string(tx.nTxIn);

	for (size_t i = 0; i < tx.vIn.size(); i++)
		message += tx.vIn[i].blockId;

	for (size_t i = 0; i < tx.vIn.size(); i++)
		message += tx.vIn[i].txId;

	for (size_t i = 0; i < tx.vIn.size(); i++)
		message += to_string(tx.vIn[i].nutxo);

	for (size_t i = 0; i < tx.vIn.size(); i++)
		message += tx.vIn[i].signature;

	for (size_t i = 0; i < tx.vOut.size(); i++)
		message += tx.vOut[i].publicId;

	for (size_t i = 0; i < tx.vOut.size(); i++)
		message += to_string(tx.vOut[i].amount);

	return hashMessage(message);
}

string cryptoHandler::concatenateVout(Transaction& tx)
{
	string rta = "";

	for (size_t i = 0; i < tx.vOut.size(); i++) {
		rta += tx.vOut[i].publicId;
	}
	for (size_t i = 0; i < tx.vOut.size(); i++) {
		rta += to_string(tx.vOut[i].amount);
	}
	return rta;
}
