#ifndef __MHD_USE_MONOCYPHER_
#define __MHD_USE_MONOCYPHER_

    #include <iostream>
    #include <string>
    #include <vector>
    #include "monocypher.h"
    #include <cstring>

    // Utility: Convert std::string key to 32-byte key (hash or pad)
    void derive_key(const std::string& input_key, uint8_t out_key[32]);

    std::string crypt_message(std::string message, std::string key);

    std::string decrypt_message(std::string encrypted, std::string key);

#endif