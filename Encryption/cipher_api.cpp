#include "cipher_api.h"
#include "cipher.hpp"
#include <cstring>



EXPORT cipher_t* cipher_create_caesar(int key) {
    Cipher* real_cipher = new CaesarCipher(key);
    void** handle = new void*; 
    *handle = static_cast<void*>(real_cipher);
    return reinterpret_cast<cipher_t*>(handle);
}


EXPORT cipher_t* cipher_create_vigenere(const char* key) {
    Cipher* real_cipher = new VigenereCipher(key);
    void** handle = new void*;
    *handle = static_cast<void*>(real_cipher);
    return reinterpret_cast<cipher_t*>(handle);
}


EXPORT char* cipher_encrypt(cipher_t* cipher, const char* text) {
    if (!cipher) return nullptr;

    void** handle = reinterpret_cast<void**>(cipher);
    Cipher* obj = static_cast<Cipher*>(*handle);
    
    std::string result = obj->encrypt(text);
    
    char* c_str = new char[result.length() + 1];
    std::strcpy(c_str, result.c_str());
    return c_str;
}


EXPORT char* cipher_decrypt(cipher_t* cipher, const char* text) {
    if (!cipher) return nullptr;
    void** handle = reinterpret_cast<void**>(cipher);
    Cipher* obj = static_cast<Cipher*>(*handle);
    
    std::string result = obj->decrypt(text);
    
    char* c_str = new char[result.length() + 1];
    std::strcpy(c_str, result.c_str());
    return c_str;
}


EXPORT void cipher_destroy(cipher_t* cipher) {
    if (!cipher) return;
    
    void** handle = reinterpret_cast<void**>(cipher);
    Cipher* obj = static_cast<Cipher*>(*handle);
    
    delete obj;
    delete handle;
}


EXPORT void cipher_free(char* str) {
    delete[] str;
}