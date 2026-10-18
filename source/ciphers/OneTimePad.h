#pragma once
#include <cstddef>
#include <string>

class OneTimePad
{
public:
    // Generates a uniformly random A-Z key using a cryptographic random source.
    std::string generateKey(std::size_t length) const;
    std::string encrypt(const std::string& plaintext, const std::string& key) const;
    std::string decrypt(const std::string& ciphertext, const std::string& key) const;
    static bool isValidKey(const std::string& key, std::size_t expectedLength);
};
