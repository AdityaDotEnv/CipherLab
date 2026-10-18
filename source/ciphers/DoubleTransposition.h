#pragma once
#include <string>

class DoubleTransposition
{
public:
    // Applies two columnar transpositions in order. Decryption reverses them.
    std::string encrypt(const std::string& plaintext,
                        const std::string& firstKey,
                        const std::string& secondKey) const;
    std::string decrypt(const std::string& ciphertext,
                        const std::string& firstKey,
                        const std::string& secondKey) const;

    static bool isValidKey(const std::string& key);
private:
    static std::string transposeEncrypt(const std::string& text, const std::string& key);
    static std::string transposeDecrypt(const std::string& text, const std::string& key);
};
