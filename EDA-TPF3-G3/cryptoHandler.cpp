#include "cryptoHandler.h"
#include "cryptoFunctions.h"

cryptoHandler::cryptoHandler()
{
	myprivateKey = generatePrivKey();
	myprivateKey.MakePublicKey(mypublicKey);
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

string cryptoHandler::signMessage(string& message)
{
	vector<byte> b = getSignature(myprivateKey,message);
	return byteToString(b);
}

bool cryptoHandler::isSignValid(string& message, string& pubKey, string& sign)
{
	//WIP
	//validateSignatureString(pubKey,message,sign);
	return true;
}

string cryptoHandler::hashMessage(string& message)
{
	return hashMessage(message);
}

bool cryptoHandler::isHashValid(string& message, string& hash)
{
	return (hash == hashMessage(message));
}
