#include <iostream>


class Cipher {
public:
    virtual std::string encrypt(const std::string& text) = 0;
    virtual std::string decrypt(const std::string& text) = 0;
    virtual ~Cipher() = default;
};


class CaesarCipher : public Cipher {
    int _key;
public:
    CaesarCipher(int key);
    std::string encrypt(const std::string& text) override;
    std::string decrypt(const std::string& text) override;
};


class VigenereCipher : public Cipher {
    std::string _key;
public:
    VigenereCipher(std::string key);
    std::string encrypt(const std::string& text) override;
    std::string decrypt(const std::string& text) override;
};