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

    namespace base64 {

        static const std::string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        inline std::string encode(const std::string& input) {
            std::string output;
            int val = 0;
            int valb = -6;
            for (unsigned char c : input) {
                val = (val << 8) + c;
                valb += 8;
                while (valb >= 0) {
                    output.push_back(base64_chars[(val >> valb) & 0x3F]);
                    valb -= 6;
                }
            }
            if (valb > -6) output.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
            while (output.size() % 4) output.push_back('=');
            return output;
        }

        inline std::string decode(const std::string& input) {
            std::vector<int> T(256, -1);
            for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;

            std::string output;
            int val = 0;
            int valb = -8;
            for (unsigned char c : input) {
                if (T[c] == -1) break;
                val = (val << 6) + T[c];
                valb += 6;
                if (valb >= 0) {
                    output.push_back(char((val >> valb) & 0xFF));
                    valb -= 8;
                }
            }
            return output;
        }

    } // namespace base64

#endif