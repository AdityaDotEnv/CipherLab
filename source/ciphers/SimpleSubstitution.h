#pragma once
#include <string>

class SimpleSubstitution
{
public:
    // key maps plaintext A..Z to the 26-letter cipher alphabet.
    std::string encrypt(const std::string& plaintext, const std::string& key) const;
    std::string decrypt(const std::string& ciphertext, const std::string& key) const;

    // Retain only ASCII letters and convert them to uppercase.
    static std::string normalize(const std::string& text);
    static bool isValidKey(const std::string& key);
};
