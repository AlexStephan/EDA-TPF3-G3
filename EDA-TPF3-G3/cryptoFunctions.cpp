#include "cryptoFunctions.h"

#define PRIVATE_KEY_CHARS 32
#define PUBLIC_KEY_CHARS 64

ECDSA<ECP, SHA256>::PrivateKey generatePrivKey()
{
	AutoSeededRandomPool autoSeededRandomPool;
	ECDSA<ECP, SHA256>::PrivateKey privateKey;

	//privateKey.Initialize(autoSeededRandomPool, ASN1::secp160r1());
	privateKey.Initialize(autoSeededRandomPool, ASN1::secp256k1());
	bool result = privateKey.Validate(autoSeededRandomPool, 3);
	if (!result)
		cout << "private key is not valid!";
	return privateKey;
}

vector<byte> getSignature(ECDSA<ECP, SHA256>::PrivateKey& privKey, string& data)
{
	AutoSeededRandomPool autoSeededRandomPool;
	ECDSA<ECP, SHA256>::Signer signer(privKey); //Creamos el signador
	bool result = signer.AccessKey().Validate(autoSeededRandomPool, 3);
	if (!result)
		cout << "failed to create signer" << result;
	size_t siglen = signer.MaxSignatureLength();
	vector<byte> signature(siglen, 0x00); //Aqui guardaremos la signature
	siglen = signer.SignMessage(autoSeededRandomPool, (const byte*)data.data(), data.size(), (byte*)signature.data());
	return signature;
}

bool verifySignature(ECDSA<ECP, SHA256>::PublicKey& pubKey, string& data, vector<byte>& signature)
{
	ECDSA<ECP, SHA256>::Verifier verifier(pubKey);
	return  verifier.VerifyMessage((const byte*)data.data(), data.size(), (const byte*)signature.data(), signature.size());
}

vector<byte> privateKeyToByte(ECDSA<ECP, SHA256>::PrivateKey& privKey)
{
	vector<byte> privKeyByteArray(PRIVATE_KEY_CHARS);
	const Integer& privateKeyInteger = privKey.GetPrivateExponent(); //La key posta
	privateKeyInteger.Encode(privKeyByteArray.data(), privateKeyInteger.ByteCount());

	return privKeyByteArray;
}

vector<byte> publicKeyToByte(ECDSA<ECP, SHA256>::PublicKey& pubKey)
{
	vector<byte> pubKeyByteArray(PUBLIC_KEY_CHARS);
	vector<byte> aux(PUBLIC_KEY_CHARS / 2);
	const ECP::Point& point = pubKey.GetPublicElement();
	point.x.Encode(aux.data(), point.x.ByteCount());
	pubKeyByteArray.clear();
	pubKeyByteArray.insert(pubKeyByteArray.end(), aux.begin(), aux.end());
	point.y.Encode(aux.data(), point.y.ByteCount());
	pubKeyByteArray.insert(pubKeyByteArray.end(), aux.begin(), aux.end());

	return pubKeyByteArray;
}

string byteToString(vector<byte>& dataToPrint)
{
	CryptoPP::HexEncoder encoder;
	string output;
	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(dataToPrint.data(), dataToPrint.size());
	encoder.MessageEnd();

	return output;
}

void copyPublicKeys(ECDSA<ECP, SHA256>::PublicKey& original, ECDSA<ECP, SHA256>::PublicKey& copy)
{
	byte buf[64 + 24];
	ArraySink sink(buf, 64 + 24);
	original.Save(sink);

	ArraySource source(buf, 64 + 24, true);
	copy.Load(source);
}

string hashMessage(string& message)
{
	SHA256 hash;
	string digest;
	StringSource s(message, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));
	return digest;
}