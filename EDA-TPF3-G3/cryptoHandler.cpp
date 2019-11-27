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
