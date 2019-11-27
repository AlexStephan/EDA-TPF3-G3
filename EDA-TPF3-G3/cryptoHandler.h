#pragma once

//       \\\\\           \\\\\           \\\\\           \\\\\    
//      \\\\\\\__o      \\\\\\\__o      \\\\\\\__o      \\\\\\\__o
//______\\\\\\\'/_______\\\\\\\'/_______\\\\\\\'/_______\\\\\\\'/_
//toda la clase

#include "string"

#include "cryptlib.h"
#include "osrng.h"
#include "eccrypto.h"
#include "oids.h"
#include "hex.h"
#include "sha3.h"
#include <string>

using namespace std;

class cryptoHandler
{
public:
	cryptoHandler(); //genera private y public keys
	string getMyPrivateKey();
	string getMyPublicKey();

	string signMessage(string& message);
	bool isSignValid(string& message, string& pubKey, string& sign);

	string hashMessage(string& message);
	bool isHashValid(string& message, string& hash);
private:

	byte header[24];
	ECDSA<ECP, SHA256>::PrivateKey myprivateKey;
	ECDSA<ECP, SHA256>::PublicKey mypublicKey;
};

