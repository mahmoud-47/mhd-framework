#include <string>
#include <sstream>
#include <iomanip>
#include "crypto/sha256.hpp"  

std::string hash_password(const std::string& password) {
    BYTE hash[32];  // SHA-256 produces 256-bit = 32-byte hashes
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, reinterpret_cast<const BYTE*>(password.c_str()), password.length());
    sha256_final(&ctx, hash);

    // Convert the hash to a hex string
    std::stringstream ss;
    for (int i = 0; i < 32; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);

    return ss.str();  // 64-character hex string
}
