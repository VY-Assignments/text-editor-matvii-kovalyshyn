#include <iostream>
#include <string>
#include "cipher_api.h"
#include <dlfcn.h>


typedef void* cipher_t;
typedef cipher_t* (*create_caesar_fn)(int);
typedef cipher_t* (*create_vigenere_fn)(const char*);
typedef char* (*encrypt_fn)(cipher_t*, const char*);
typedef char* (*decrypt_fn)(cipher_t*, const char*);
typedef void (*destroy_fn)(cipher_t*);
typedef void (*free_fn)(char*);


int main() {

    void* handle = dlopen("./libcipher.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error library loading: " << dlerror() << std::endl;
        return 1;
    }

    auto cipher_create_caesar = (create_caesar_fn)dlsym(handle, "cipher_create_caesar");
    auto cipher_create_vigenere = (create_vigenere_fn)dlsym(handle, "cipher_create_vigenere");
    auto cipher_encrypt = (encrypt_fn)dlsym(handle, "cipher_encrypt");
    auto cipher_decrypt = (decrypt_fn)dlsym(handle, "cipher_decrypt");
    auto cipher_destroy = (destroy_fn)dlsym(handle, "cipher_destroy");
    auto cipher_free = (free_fn)dlsym(handle, "cipher_free");

    if (!cipher_create_caesar || !cipher_create_vigenere || !cipher_encrypt || 
        !cipher_decrypt || !cipher_destroy || !cipher_free) {
        std::cerr << "Functions are not found." << std::endl;
        dlclose(handle);
        return 1;
    }

    // printf("[DEBUG] Dynamic Library Loading Success!\n");
    // printf("  -> libcipher.so handle address: %p\n", handle);
    // printf("  -> cipher_create_caesar fn pointer: %p\n", (void*)cipher_create_caesar);
    // printf("  -> cipher_encrypt fn pointer: %p\n", (void*)cipher_encrypt);

    while(true) {
        short userCipher;
        printf("\nChoose a cipher: caesar(1) or vigenere(2). Exit(0).\n");
        printf("-> ");
        scanf("%d", &userCipher);
        while(getchar() != '\n');
        if (userCipher == 1) {
            int key = 0;
            printf("Choose a key: ");
            scanf("%d", &key);    
            while(getchar() != '\n');            

            std::string userText = "";
            printf("\nEnter text: ");
            std::getline(std::cin, userText);    
            if (userText.length() == 0) {
                break;
            }
            cipher_t* caesar = cipher_create_caesar(key);

            printf("\nCaeser.\n");
            char* encrypted_caeser = cipher_encrypt(caesar, userText.c_str());
            printf("encrypted = %s\n", encrypted_caeser);
            char* decrypted_caeser = cipher_decrypt(caesar, encrypted_caeser);
            printf("decrypted = %s\n", decrypted_caeser);
        
            cipher_free(encrypted_caeser);
            cipher_free(decrypted_caeser);
            cipher_destroy(caesar);
        }
        else if (userCipher == 2) {
            std::string key = "a";
            printf("Choose a key: ");
            std::getline(std::cin, key);
            
            std::string userText = "";
            printf("\nEnter text: ");
            std::getline(std::cin, userText);    
            if (userText.length() == 0) {
                break;
            }
            cipher_t* vigenere = cipher_create_vigenere(key.c_str());

            printf("\nVigenere.\n");
            char* encrypted_vigenere = cipher_encrypt(vigenere, userText.c_str());
            printf("encrypted = %s\n", encrypted_vigenere);
            char* decrypted_vigenere = cipher_decrypt(vigenere, encrypted_vigenere);
            printf("decrypted = %s\n", decrypted_vigenere);

            cipher_free(encrypted_vigenere);
            cipher_free(decrypted_vigenere);
            cipher_destroy(vigenere);
        }   
        else if (userCipher == 0) {
            break;
        }
    }    
    dlclose(handle);
    return 0;
}
