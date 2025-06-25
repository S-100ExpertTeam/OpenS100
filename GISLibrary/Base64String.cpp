#include "stdafx.h"
#include "Base64String.h"

Base64String Base64String::Encode() {
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