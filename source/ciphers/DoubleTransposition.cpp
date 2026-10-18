#include "DoubleTransposition.h"
#include "SimpleSubstitution.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace
{
    std::string uppercaseKey(const std::string& key)
    {
        std::string result;
        result.reserve(key.size());
        for (unsigned char ch : key)
        {
            if (ch >= 'a' && ch <= 'z')
                result.push_back(static_cast<char>(ch - 'a' + 'A'));
            else
                result.push_back(static_cast<char>(ch));
        }
        return result;
    }

    std::vector<std::size_t> columnOrder(const std::string& rawKey)
    {
        const auto key = uppercaseKey(rawKey);
        std::vector<std::size_t> order(key.size());
        std::iota(order.begin(), order.end(), 0);
        std::stable_sort(order.begin(), order.end(), [&key](std::size_t a, std::size_t b)
        {
            return key[a] < key[b];
        });
        return order;
    }
}

bool DoubleTransposition::isValidKey(const std::string& rawKey)
{
    const auto key = uppercaseKey(rawKey);
    return !key.empty() && std::all_of(key.begin(), key.end(), [](unsigned char ch)
    {
        return ch >= 'A' && ch <= 'Z';
    });
}

std::string DoubleTransposition::transposeEncrypt(const std::string& rawText, const std::string& rawKey)
{
    if (!isValidKey(rawKey))
        throw std::invalid_argument("Transposition keys must be non-empty alphabetic words.");

    const auto text = SimpleSubstitution::normalize(rawText);
    const auto key = uppercaseKey(rawKey);
    const auto order = columnOrder(key);
    std::string output;
    output.reserve(text.size());

    // Write row-wise into a ragged rectangle, then read columns in key order.
    for (const auto column : order)
        for (std::size_t i = column; i < text.size(); i += key.size())
            output.push_back(text[i]);

    return output;
}

std::string DoubleTransposition::transposeDecrypt(const std::string& rawText, const std::string& rawKey)
{
    if (!isValidKey(rawKey))
        throw std::invalid_argument("Transposition keys must be non-empty alphabetic words.");

    const auto text = SimpleSubstitution::normalize(rawText);
    const auto key = uppercaseKey(rawKey);
    const auto order = columnOrder(key);
    const auto columns = key.size();
    const auto rows = (text.size() + columns - 1) / columns;
    const auto fullColumns = text.size() % columns;
    const auto longColumns = fullColumns == 0 ? columns : fullColumns;

    std::vector<std::string> columnData(columns);
    std::size_t cursor = 0;
    for (const auto column : order)
    {
        const std::size_t length = rows == 0 ? 0 : (column < longColumns ? rows : rows - 1);
        columnData[column] = text.substr(cursor, length);
        cursor += length;
    }

    std::string output;
    output.reserve(text.size());
    for (std::size_t row = 0; row < rows; ++row)
        for (std::size_t column = 0; column < columns; ++column)
            if (row < columnData[column].size())
                output.push_back(columnData[column][row]);

    return output;
}

std::string DoubleTransposition::encrypt(const std::string& plaintext,
                                         const std::string& firstKey,
                                         const std::string& secondKey) const
{
    return transposeEncrypt(transposeEncrypt(plaintext, firstKey), secondKey);
}

std::string DoubleTransposition::decrypt(const std::string& ciphertext,
                                         const std::string& firstKey,
                                         const std::string& secondKey) const
{
    return transposeDecrypt(transposeDecrypt(ciphertext, secondKey), firstKey);
}
