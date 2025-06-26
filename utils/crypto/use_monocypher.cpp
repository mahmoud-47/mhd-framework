#include "use_monocypher.hpp"

// Utility: Convert std::string key to 32-byte key (hash or pad)
void derive_key(const std::string& input_key, uint8_t out_key[32]) {
    std::fill(out_key, out_key + 32, 0);
    std::memcpy(out_key, input_key.data(), std::min(size_t(32), input_key.size()));
}

std::string crypt_message(std::string message, std::string key) {
    uint8_t sym_key[32];
    derive_key(key, sym_key);

    // In production, use a secure random 24-byte nonce!
    uint8_t nonce[24] = {0};
    std::memcpy(nonce, "static-nonce-example", 21);  // Replace with random nonce

    std::vector<uint8_t> ciphertext(message.size());
    uint8_t mac[16];

    // AEAD: Authenticated Encryption with Additional Data
    crypto_aead_lock(ciphertext.data(), mac, sym_key, nonce,
                     nullptr, 0,  // No associated data
                     reinterpret_cast<const uint8_t*>(message.data()), message.size());

    std::string output;
    output.append(reinterpret_cast<char*>(nonce), 24);
    output.append(reinterpret_cast<char*>(mac), 16);
    output.append(reinterpret_cast<char*>(ciphertext.data()), ciphertext.size());

    return output;
}

std::string decrypt_message(std::string encrypted, std::string key) {
    if (encrypted.size() < 40) return ""; // invalid

    uint8_t sym_key[32];
    derive_key(key, sym_key);

    const uint8_t* nonce  = reinterpret_cast<const uint8_t*>(encrypted.data());
    const uint8_t* mac    = reinterpret_cast<const uint8_t*>(encrypted.data() + 24);
    const uint8_t* cipher = reinterpret_cast<const uint8_t*>(encrypted.data() + 40);
    size_t cipher_len = encrypted.size() - 40;

    std::vector<uint8_t> decrypted(cipher_len);

    int result = crypto_aead_unlock(decrypted.data(), mac, sym_key, nonce,
                                    nullptr, 0,  // No associated data
                                    cipher, cipher_len);

    if (result != 0) {
        return ""; // authentication failed
    }

    return std::string(reinterpret_cast<char*>(decrypted.data()), decrypted.size());
}