#pragma once
//#include <afx.h>
#include <openssl/evp.h>
#include <openssl/dsa.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <string>
#include <stdexcept>
#include <vector>
#include <openssl/buffer.h>

class Base64String : public std::string {
public:
    using std::string::string;
    using std::string::operator=;

    Base64String Encode();



   /* Base64String Encode() {
        DWORD encodedLen = 0;
        if (!CryptBinaryToStringA(
            reinterpret_cast<const BYTE*>(this->c_str()),
            this->length(),
            CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
            nullptr,
            &encodedLen)) {
            throw std::runtime_error("Get length error.");
        }

        std::vector<CHAR> encodedData(encodedLen);
        if (!CryptBinaryToStringA(
            reinterpret_cast<const BYTE*>(this->c_str()),
            this->length(),
            CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
            encodedData.data(),
            &encodedLen)) {
            throw std::runtime_error("Encoding error.");
        }

        return Base64String(std::string(encodedData.begin(), encodedData.end()));
    }

    std::string Decode() {
        DWORD decodedLen = 0;
        if (!CryptStringToBinaryA(
            this->c_str(),
            0,
            CRYPT_STRING_BASE64,
            nullptr,
            &decodedLen,
            nullptr,
            nullptr)) {
            throw std::runtime_error("Get length error.");
        }

        std::vector<BYTE> decodedData(decodedLen);
        if (!CryptStringToBinaryA(
            this->c_str(),
            0,
            CRYPT_STRING_BASE64,
            decodedData.data(),
            &decodedLen,
            nullptr,
            nullptr)) {
            throw std::runtime_error("Decoding error.");
        }

        return std::string(reinterpret_cast<char*>(decodedData.data()), decodedLen);
    }*/
};