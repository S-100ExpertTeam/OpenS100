#pragma once
#include <stdexcept>
#include <string>

using namespace std;

namespace S100 
{
	enum class S100_SE_DigitalSignatureReference
	{
		RSA = 1,
		DSA,
		ECDSA,
		ECDSA_224_SHA2_224,
		ECDSA_224_SHA3_224,
		ECDSA_256_SHA2_256,
		ECDSA_256_SHA3_256,
		ECDSA_384_SHA2,
		ECDSA_384_SHA3,
		AES_128,
		AES_192,
		AES_256
	};

	inline string S100_SE_DigitalSignatureReferenceToString(S100_SE_DigitalSignatureReference category) {
		switch (category) {
		case S100_SE_DigitalSignatureReference::RSA: return "RSA";
		case S100_SE_DigitalSignatureReference::DSA: return "DSA";
		case S100_SE_DigitalSignatureReference::ECDSA: return "ECDSA";
		case S100_SE_DigitalSignatureReference::ECDSA_224_SHA2_224: return "ECDSA-224-SHA2-224";
		case S100_SE_DigitalSignatureReference::ECDSA_224_SHA3_224: return "ECDSA-224-SHA3-224";
		case S100_SE_DigitalSignatureReference::ECDSA_256_SHA2_256: return "ECDSA-256-SHA2-256";
		case S100_SE_DigitalSignatureReference::ECDSA_256_SHA3_256: return "ECDSA-256-SHA3-256";
		case S100_SE_DigitalSignatureReference::ECDSA_384_SHA2: return "ECDSA-384-SHA2";
		case S100_SE_DigitalSignatureReference::ECDSA_384_SHA3: return "ECDSA-384-SHA3";
		case S100_SE_DigitalSignatureReference::AES_128: return "AES-128";
		case S100_SE_DigitalSignatureReference::AES_192: return "AES-192";
		case S100_SE_DigitalSignatureReference::AES_256: return "AES-256";

		default: throw invalid_argument("Unknown category");
		}
	}

	inline S100_SE_DigitalSignatureReference S100_SE_DigitalSignatureReferenceFromString(const string& categoryName) {
		if (categoryName == "RSA") return S100_SE_DigitalSignatureReference::RSA;
		if (categoryName == "DSA") return S100_SE_DigitalSignatureReference::DSA;
		if (categoryName == "ECDSA") return S100_SE_DigitalSignatureReference::ECDSA;
		if (categoryName == "ECDSA-224-SHA2-224") return S100_SE_DigitalSignatureReference::ECDSA_224_SHA2_224;
		if (categoryName == "ECDSA-224-SHA3-224") return S100_SE_DigitalSignatureReference::ECDSA_224_SHA3_224;
		if (categoryName == "ECDSA-256-SHA2-256") return S100_SE_DigitalSignatureReference::ECDSA_256_SHA2_256;
		if (categoryName == "ECDSA-256-SHA3-256") return S100_SE_DigitalSignatureReference::ECDSA_256_SHA3_256;
		if (categoryName == "ECDSA-384-SHA2") return S100_SE_DigitalSignatureReference::ECDSA_384_SHA2;
		if (categoryName == "ECDSA-384-SHA3") return S100_SE_DigitalSignatureReference::ECDSA_384_SHA3;
		if (categoryName == "AES-128") return S100_SE_DigitalSignatureReference::AES_128;
		if (categoryName == "AES-192") return S100_SE_DigitalSignatureReference::AES_192;
		if (categoryName == "AES-256") return S100_SE_DigitalSignatureReference::AES_256;
		throw invalid_argument("Unknown category name");
	}
}