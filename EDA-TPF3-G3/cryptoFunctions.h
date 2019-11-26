#pragma once

#include "cryptlib.h"
#include "osrng.h"
#include "eccrypto.h"
#include "oids.h"
#include "hex.h"
#include "sha3.h"
#include <string>

using namespace std;
using namespace CryptoPP;

ECDSA<ECP, SHA256>::PrivateKey generatePrivKey();
vector<byte> getSignature(ECDSA<ECP, SHA256>::PrivateKey& privKey, string& data);
bool verifySignature(ECDSA<ECP, SHA256>::PublicKey& pubKey, string& data, vector<byte>& signature);
//bool verifySignatureString(string &pubKey, string &data, vector<byte> &signature); //(WIP)

vector<byte> privateKeyToByte(ECDSA<ECP, SHA256>::PrivateKey& privKey);
vector<byte> publicKeyToByte(ECDSA<ECP, SHA256>::PublicKey& pubKey);
string byteToString(vector<byte>& dataToPrint);
//void stringToByte(string& dataToPrint,byte output[64]); //(WIP)

void copyPublicKeys(ECDSA<ECP, SHA256>::PublicKey& original, ECDSA<ECP, SHA256>::PublicKey& copy); //be careful!! (might explode)

string hashMessage(string& message);