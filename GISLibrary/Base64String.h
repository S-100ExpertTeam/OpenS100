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

        // ���ڵ� �������� ���� ���ڸ� ���� �ʵ��� �մϴ�.
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

        // �����͸� Base64�� ���ڵ��մϴ�.
        BIO_write(bio, this->c_str(), this->length());
        BIO_flush(bio);
        BIO_get_mem_ptr(bio, &bufferPtr);
        BIO_set_close(bio, BIO_NOCLOSE); // bufferPtr�� �������� �ʵ��� ����

        // ���ۿ��� Base64 ���ڵ��� ���ڿ��� �����մϴ�.
        std::string base64Text(bufferPtr->data, bufferPtr->length);

        // BIO ü���� �����ϰ� �޸𸮸� �����մϴ�.
        BIO_free_all(bio);

        return (Base64String)base64Text;
    }

    //Base64String Decode() {
    //    BIO* bio, * b64;
    //    const int decodeBufferSize = (this->length() * 3) / 4; // Base64 ���ڵ��� ���� ����
    //    char* buffer = (char*)malloc(decodeBufferSize + 1); // +1 for NULL termination
    //    memset(buffer, 0, decodeBufferSize + 1);

    //    b64 = BIO_new(BIO_f_base64());
    //    bio = BIO_new_mem_buf(this->data(), this->length());
    //    bio = BIO_push(b64, bio);

    //    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // ���� ���� ����

    //    int length = BIO_read(bio, buffer, this->length());

    //    std::string decodedText(buffer, length); // ��Ȯ�� ������ ���ڿ� ����

    //    free(buffer); // �Ҵ�� �޸� ����
    //    BIO_free_all(bio); // BIO ü�� ����

    //    return Base64String(decodedText); // ��ȯ�� ���ڿ��� Base64String ��ü ����
    //}

    //Base64String Decode() {
    //    EVP_PKEY* pkey = decodePrivateKeyPEM(*this);
    //    X509* x509 = DecodeBase64ToX509(*this);

    //    // ��� ��, �Ҵ�� �ڿ��� �����մϴ�.
    //    EVP_PKEY_free(pkey);
    //    X509_free(x509);

    //    return (Base64String)"";
    //}


    //// PEM ���ڵ��� ���� Ű�� EVP_PKEY ����ü�� ���ڵ��ϴ� �Լ�
    //EVP_PKEY* decodePrivateKeyPEM(const std::string& private_key_pem) {
    //    BIO* bio_private = BIO_new_mem_buf(private_key_pem.data(), -1);
    //    EVP_PKEY* pkey = PEM_read_bio_PrivateKey(bio_private, NULL, NULL, NULL);
    //    BIO_free(bio_private);
    //    return pkey; // Caller must free the EVP_PKEY
    //}

    //// PEM ���ڵ��� �������� X509 ����ü�� ���ڵ��ϴ� �Լ�
    //X509* decodeCertificatePEM(const std::string& certificate_pem) {
    //    BIO* bio_cert = BIO_new_mem_buf(certificate_pem.data(), -1);
    //    X509* x509 = PEM_read_bio_X509(bio_cert, NULL, NULL, NULL);
    //    BIO_free(bio_cert);
    //    return x509; // Caller must free the X509
    //}

    //X509* DecodeBase64ToX509(const std::string& base64_encoded_cert) {
    //    // Base64 ���ڵ��� ���� BIO ü���� �����մϴ�.
    //    BIO* bio = BIO_new_mem_buf(base64_encoded_cert.data(), -1); // -1�� ���ڿ��� null-terminated���� �ǹ��մϴ�.
    //    BIO* b64 = BIO_new(BIO_f_base64());
    //    bio = BIO_push(b64, bio);
    //    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // ���๮�� ����

    //    // Base64 ���ڵ��� �����ϰ� DER ���ڵ��� �������� �н��ϴ�.
    //    unsigned char buffer[4096] = {}; // �������� ũ�Ⱑ �� ���ۺ��� Ŭ �� ���ٰ� �����մϴ�.
    //    int len = BIO_read(bio, buffer, sizeof(buffer));
    //    if (len <= 0) {
    //        BIO_free_all(bio);
    //        return nullptr;
    //    }

    //    // DER ���ڵ��� �������� X509 ����ü�� ��ȯ�մϴ�.
    //    const unsigned char* p = buffer;
    //    X509* cert = d2i_X509(NULL, &p, len);
    //    BIO_free_all(bio);

    //    return cert; // ȣ���ڰ� X509_free()�� ȣ���Ͽ� �޸𸮸� �����ؾ� �մϴ�.
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