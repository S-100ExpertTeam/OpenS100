#pragma once
#include <stdexcept>
#include <string>

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

    std::string S100_SE_DigitalSignatureReferenceToString(S100_SE_DigitalSignatureReference category);

    S100_SE_DigitalSignatureReference S100_SE_DigitalSignatureReferenceFromString(const std::string& categoryName);
}
