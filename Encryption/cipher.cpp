#include <iostream>
#include <cstring>
#include <cctype>
#include "cipher.hpp"


CaesarCipher::CaesarCipher(int key) {
    if (key > 26 || key < -26) {
        key %= 26;
    }
    _key = key;
}


std::string CaesarCipher::encrypt(const std::string& text) {
    std::string encrypted = text;
    for (int i = 0; i < text.length(); i++) {
        if (encrypted[i] >= 'A' && encrypted[i] <= 'Z') {
            if (encrypted[i] + _key > 'Z') {
                encrypted[i] = 'A' - 1 + (encrypted[i] + _key - 'Z');
            }
            else if (encrypted[i] + _key < 'A') {
                encrypted[i] = 'Z' + 1 - ('A' - (encrypted[i] + _key));
            }
            else {
                encrypted[i] = encrypted[i] + _key;
            }
            
        }
        else if (encrypted[i] >= 'a' && encrypted[i] <= 'z') {
            if (encrypted[i] + _key > 'z') {
                encrypted[i] = 'a' - 1 + (encrypted[i] + _key - 'z');
            }
            else if (encrypted[i] + _key < 'a') {
                encrypted[i] = 'z' + 1 - ('a' - (encrypted[i] + _key));
            }
            else {
                encrypted[i] = encrypted[i] + _key;
            }  
        } 
    }
    return encrypted;
}


std::string CaesarCipher::decrypt(const std::string& text) {
    std::string decrypted;
    CaesarCipher caeser(-_key);
    decrypted = caeser.encrypt(text);
    return decrypted;
}


VigenereCipher::VigenereCipher(std::string key) {
    _key = key;
    for (int i = 0; i < _key.length(); i++) {
        _key[i] = std::toupper(_key[i]) - 'A';
    }
}


std::string VigenereCipher::encrypt(const std::string& text) {
    std::string encrypted = text;
    int j = 0;
    for (int i = 0; i < text.length(); i++) {
        if (j >= _key.length()) {
            j = 0;
        }
        if (encrypted[i] >= 'A' && encrypted[i] <= 'Z') {
            if (encrypted[i] + _key[j] > 'Z') {
                encrypted[i] = 'A' - 1 + (encrypted[i] + _key[j] - 'Z');
            }
            else if (encrypted[i] + _key[j] < 'A') {
                encrypted[i] = 'Z' + 1 - ('A' - (encrypted[i] + _key[j]));
            }
            else {
                encrypted[i] = encrypted[i] + _key[j];
            }
            j++;
        }
        else if (encrypted[i] >= 'a' && encrypted[i] <= 'z') {
            if (encrypted[i] + _key[j] > 'z') {
                encrypted[i] = 'a' - 1 + (encrypted[i] + _key[j] - 'z');
            }
            else if (encrypted[i] + _key[j] < 'a') {
                encrypted[i] = 'z' + 1 - ('a' - (encrypted[i] + _key[j]));
            }
            else {
                encrypted[i] = encrypted[i] + _key[j];
            }  
            j++;
        }  
         
    }

    return encrypted;
}


std::string VigenereCipher::decrypt(const std::string& text) {
    std::string decrypted = text;
    int j = 0;
    for (int i = 0; i < text.length(); i++) {
        if (j >= _key.length()) {
            j = 0;
        }
        if (decrypted[i] >= 'A' && decrypted[i] <= 'Z') {
            if (decrypted[i] - _key[j] > 'Z') {
                decrypted[i] = 'A' - 1 + (decrypted[i] - _key[j] - 'Z');
            }
            else if (decrypted[i] - _key[j] < 'A') {
                decrypted[i] = 'Z' + 1 - ('A' - (decrypted[i] - _key[j]));
            }
            else {
                decrypted[i] = decrypted[i] - _key[j];
            }
            j++;
        }
        else if (decrypted[i] >= 'a' && decrypted[i] <= 'z') {
            if (decrypted[i] - _key[j] > 'z') {
                decrypted[i] = 'a' - 1 + (decrypted[i] - _key[j] - 'z');
            }
            else if (decrypted[i] - _key[j] < 'a') {
                decrypted[i] = 'z' + 1 - ('a' - (decrypted[i] - _key[j]));
            }
            else {
                decrypted[i] = decrypted[i] - _key[j];
            }  
            j++;
        }  
         
    }

    return decrypted;
}