#include "OneTimePad.h"
#include "SimpleSubstitution.h"
#include <random>
#include <stdexcept>

std::string OneTimePad::generateKey(std::size_t length) const
{
    std::random_device rd;
    std::uniform_int_distribution<int> distribution(0, 25);
    std::string key;
    key.reserve(length);
    for (std::size_t i = 0; i < length; ++i)
        key.push_back(static_cast<char>('A' + distribution(rd)));
    return key;
}

bool OneTimePad::isValidKey(const std::string& rawKey, std::size_t expectedLength)
{
    const auto key = SimpleSubstitution::normalize(rawKey);
    if (key.size() != expectedLength || rawKey.size() != expectedLength)
        return false;
    for (unsigned char ch : rawKey)
        if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
            return false;
    return true;
}

std::string OneTimePad::encrypt(const std::string& rawPlaintext, const std::string& rawKey) const
{
    const auto plaintext = SimpleSubstitution::normalize(rawPlaintext);
    const auto key = SimpleSubstitution::normalize(rawKey);
    if (key.size() != plaintext.size() || rawKey.size() != plaintext.size())
        throw std::invalid_argument("OTP key must contain exactly one letter per normalized plaintext letter.");

    std::string ciphertext;
    ciphertext.reserve(plaintext.size());
    for (std::size_t i = 0; i < plaintext.size(); ++i)
    {
        const int p = plaintext[i] - 'A';
        const int k = key[i] - 'A';
        ciphertext.push_back(static_cast<char>('A' + ((p + k) % 26)));
    }
    return ciphertext;
}

std::string OneTimePad::decrypt(const std::string& rawCiphertext, const std::string& rawKey) const
{
    const auto ciphertext = SimpleSubstitution::normalize(rawCiphertext);
    const auto key = SimpleSubstitution::normalize(rawKey);
    if (key.size() != ciphertext.size() || rawKey.size() != ciphertext.size())
        throw std::invalid_argument("OTP key must contain exactly one letter per ciphertext letter.");

    std::string plaintext;
    plaintext.reserve(ciphertext.size());
    for (std::size_t i = 0; i < ciphertext.size(); ++i)
    {
        const int c = ciphertext[i] - 'A';
        const int k = key[i] - 'A';
        plaintext.push_back(static_cast<char>('A' + ((c - k + 26) % 26)));
    }
    return plaintext;
}
