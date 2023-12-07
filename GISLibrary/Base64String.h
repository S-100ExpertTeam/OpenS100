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

    Base64String Encode() {
        BIO* bio, * b64;
        BUF_MEM* bufferPtr;

        b64 = BIO_new(BIO_f_base64());
        bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);

        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

        BIO_write(bio, this->c_str(), this->length());
        BIO_flush(bio);
        BIO_get_mem_ptr(bio, &bufferPtr);
        BIO_set_close(bio, BIO_NOCLOSE); 

        std::string base64Text(bufferPtr->data, bufferPtr->length);

        BIO_free_all(bio);

        return (Base64String)base64Text;
    }





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