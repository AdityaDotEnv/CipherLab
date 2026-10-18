#include "SimpleSubstitution.h"
#include <array>
#include <stdexcept>

namespace
{
    std::string uppercaseAscii(const std::string& text)
    {
        std::string result;
        result.reserve(text.size());
        for (unsigned char ch : text)
        {
            if (ch >= 'a' && ch <= 'z')
                result.push_back(static_cast<char>(ch - 'a' + 'A'));
            else
                result.push_back(static_cast<char>(ch));
        }
        return result;
    }
}

std::string SimpleSubstitution::normalize(const std::string& text)
{
    std::string result;
    result.reserve(text.size());
    for (unsigned char ch : text)
    {
        if (ch >= 'a' && ch <= 'z')
            result.push_back(static_cast<char>(ch - 'a' + 'A'));
        else if (ch >= 'A' && ch <= 'Z')
            result.push_back(static_cast<char>(ch));
    }
    return result;
}

bool SimpleSubstitution::isValidKey(const std::string& rawKey)
{
    const auto key = uppercaseAscii(rawKey);
    if (key.size() != 26)
        return false;

    std::array<bool, 26> seen {};
    for (unsigned char ch : key)
    {
        if (ch < 'A' || ch > 'Z')
            return false;
        const auto index = static_cast<std::size_t>(ch - 'A');
        if (seen[index])
            return false;
        seen[index] = true;
    }
    return true;
}

std::string SimpleSubstitution::encrypt(const std::string& plaintext, const std::string& rawKey) const
{
    const auto key = uppercaseAscii(rawKey);
    if (!isValidKey(key))
        throw std::invalid_argument("Substitution key must contain each letter A-Z exactly once.");

    const auto input = normalize(plaintext);
    std::string output;
    output.reserve(input.size());
    for (char ch : input)
        output.push_back(key[static_cast<std::size_t>(ch - 'A')]);
    return output;
}

std::string SimpleSubstitution::decrypt(const std::string& ciphertext, const std::string& rawKey) const
{
    const auto key = uppercaseAscii(rawKey);
    if (!isValidKey(key))
        throw std::invalid_argument("Substitution key must contain each letter A-Z exactly once.");

    std::array<char, 26> inverse {};
    for (std::size_t i = 0; i < 26; ++i)
        inverse[static_cast<std::size_t>(key[i] - 'A')] = static_cast<char>('A' + i);

    const auto input = normalize(ciphertext);
    std::string output;
    output.reserve(input.size());
    for (char ch : input)
        output.push_back(inverse[static_cast<std::size_t>(ch - 'A')]);
    return output;
}
