#pragma once
#include <afx.h>
#include <openssl/evp.h>
#include <openssl/dsa.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include<string>
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

        // 인코딩 과정에서 개행 문자를 넣지 않도록 합니다.
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

        // 데이터를 Base64로 인코딩합니다.
        BIO_write(bio, this->c_str(), this->length());
        BIO_flush(bio);
        BIO_get_mem_ptr(bio, &bufferPtr);
        BIO_set_close(bio, BIO_NOCLOSE); // bufferPtr를 해제하지 않도록 설정

        // 버퍼에서 Base64 인코딩된 문자열을 복사합니다.
        std::string base64Text(bufferPtr->data, bufferPtr->length);

        // BIO 체인을 해제하고 메모리를 해제합니다.
        BIO_free_all(bio);

        return (Base64String)base64Text;
    }

    //Base64String Decode() {
    //    BIO* bio, * b64;
    //    const int decodeBufferSize = (this->length() * 3) / 4; // Base64 디코딩된 길이 추정
    //    char* buffer = (char*)malloc(decodeBufferSize + 1); // +1 for NULL termination
    //    memset(buffer, 0, decodeBufferSize + 1);

    //    b64 = BIO_new(BIO_f_base64());
    //    bio = BIO_new_mem_buf(this->data(), this->length());
    //    bio = BIO_push(b64, bio);

    //    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // 개행 문자 제거

    //    int length = BIO_read(bio, buffer, this->length());

    //    std::string decodedText(buffer, length); // 정확한 길이의 문자열 생성

    //    free(buffer); // 할당된 메모리 해제
    //    BIO_free_all(bio); // BIO 체인 해제

    //    return Base64String(decodedText); // 변환된 문자열로 Base64String 객체 생성
    //}

    //Base64String Decode() {
    //    EVP_PKEY* pkey = decodePrivateKeyPEM(*this);
    //    X509* x509 = DecodeBase64ToX509(*this);

    //    // 사용 후, 할당된 자원을 해제합니다.
    //    EVP_PKEY_free(pkey);
    //    X509_free(x509);

    //    return (Base64String)"";
    //}


    //// PEM 인코딩된 개인 키를 EVP_PKEY 구조체로 디코딩하는 함수
    //EVP_PKEY* decodePrivateKeyPEM(const std::string& private_key_pem) {
    //    BIO* bio_private = BIO_new_mem_buf(private_key_pem.data(), -1);
    //    EVP_PKEY* pkey = PEM_read_bio_PrivateKey(bio_private, NULL, NULL, NULL);
    //    BIO_free(bio_private);
    //    return pkey; // Caller must free the EVP_PKEY
    //}

    //// PEM 인코딩된 인증서를 X509 구조체로 디코딩하는 함수
    //X509* decodeCertificatePEM(const std::string& certificate_pem) {
    //    BIO* bio_cert = BIO_new_mem_buf(certificate_pem.data(), -1);
    //    X509* x509 = PEM_read_bio_X509(bio_cert, NULL, NULL, NULL);
    //    BIO_free(bio_cert);
    //    return x509; // Caller must free the X509
    //}

    //X509* DecodeBase64ToX509(const std::string& base64_encoded_cert) {
    //    // Base64 디코딩을 위한 BIO 체인을 생성합니다.
    //    BIO* bio = BIO_new_mem_buf(base64_encoded_cert.data(), -1); // -1은 문자열이 null-terminated임을 의미합니다.
    //    BIO* b64 = BIO_new(BIO_f_base64());
    //    bio = BIO_push(b64, bio);
    //    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // 개행문자 무시

    //    // Base64 디코딩을 수행하고 DER 인코딩된 인증서를 읽습니다.
    //    unsigned char buffer[4096] = {}; // 인증서의 크기가 이 버퍼보다 클 수 없다고 가정합니다.
    //    int len = BIO_read(bio, buffer, sizeof(buffer));
    //    if (len <= 0) {
    //        BIO_free_all(bio);
    //        return nullptr;
    //    }

    //    // DER 인코딩된 인증서를 X509 구조체로 변환합니다.
    //    const unsigned char* p = buffer;
    //    X509* cert = d2i_X509(NULL, &p, len);
    //    BIO_free_all(bio);

    //    return cert; // 호출자가 X509_free()를 호출하여 메모리를 해제해야 합니다.
    //}






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