#include "DoubleTransposition.h"
#include "SimpleSubstitution.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace
{
    std::vector<std::size_t> parsePermutation(const std::string& raw,
                                               const char* label)
    {
        std::vector<std::size_t> permutation;
        std::istringstream input(raw);
        std::string token;

        while (std::getline(input, token, ','))
        {
            const auto first = token.find_first_not_of(" \t\r\n");
            const auto last = token.find_last_not_of(" \t\r\n");
            if (first == std::string::npos)
                throw std::invalid_argument(std::string(label) + " permutation contains an empty entry.");

            const auto trimmed = token.substr(first, last - first + 1);
            std::size_t consumed = 0;
            unsigned long value = 0;
            try
            {
                value = std::stoul(trimmed, &consumed);
            }
            catch (...)
            {
                throw std::invalid_argument(std::string(label) + " permutation must contain comma-separated integers.");
            }

            if (consumed != trimmed.size() || value == 0)
                throw std::invalid_argument(std::string(label) + " permutation values must be positive integers.");

            permutation.push_back(static_cast<std::size_t>(value));
        }

        if (permutation.empty())
            throw std::invalid_argument(std::string(label) + " permutation cannot be empty.");

        std::vector<bool> seen(permutation.size() + 1, false);
        for (const auto value : permutation)
        {
            if (value > permutation.size() || seen[value])
                throw std::invalid_argument(std::string(label) +
                    " permutation must contain each integer from 1 to its length exactly once.");
            seen[value] = true;
        }

        return permutation;
    }

    void validateDimensions(const std::string& text,
                            const std::vector<std::size_t>& rows,
                            const std::vector<std::size_t>& columns)
    {
        if (rows.size() > static_cast<std::size_t>(-1) / columns.size()
            || text.size() != rows.size() * columns.size())
        {
            throw std::invalid_argument(
                "Normalized plaintext/ciphertext length must equal row-count x column-count "
                "(the permutation matrix must be rectangular).");
        }
    }
}

bool DoubleTransposition::isValidPermutation(const std::string& rawPermutation)
{
    try
    {
        (void) parsePermutation(rawPermutation, "Permutation");
        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::string DoubleTransposition::encrypt(const std::string& rawText,
                                         const std::string& rawRowPermutation,
                                         const std::string& rawColumnPermutation) const
{
    const auto text = SimpleSubstitution::normalize(rawText);
    const auto rows = parsePermutation(rawRowPermutation, "Row");
    const auto columns = parsePermutation(rawColumnPermutation, "Column");
    validateDimensions(text, rows, columns);

    const auto rowCount = rows.size();
    const auto columnCount = columns.size();

    std::vector<std::vector<char>> matrix(rowCount, std::vector<char>(columnCount));
    for (std::size_t r = 0; r < rowCount; ++r)
        for (std::size_t c = 0; c < columnCount; ++c)
            matrix[r][c] = text[r * columnCount + c];

    // Permutation values are 1-based. Each output row/column selects
    // the corresponding original row/column.
    std::string ciphertext;
    ciphertext.reserve(text.size());
    for (const auto rowValue : rows)
        for (const auto columnValue : columns)
            ciphertext.push_back(matrix[rowValue - 1][columnValue - 1]);

    return ciphertext;
}

std::string DoubleTransposition::decrypt(const std::string& rawText,
                                         const std::string& rawRowPermutation,
                                         const std::string& rawColumnPermutation) const
{
    const auto text = SimpleSubstitution::normalize(rawText);
    const auto rows = parsePermutation(rawRowPermutation, "Row");
    const auto columns = parsePermutation(rawColumnPermutation, "Column");
    validateDimensions(text, rows, columns);

    const auto rowCount = rows.size();
    const auto columnCount = columns.size();

    std::vector<std::vector<char>> original(rowCount, std::vector<char>(columnCount));
    std::size_t cursor = 0;
    for (const auto rowValue : rows)
        for (const auto columnValue : columns)
            original[rowValue - 1][columnValue - 1] = text[cursor++];

    std::string plaintext;
    plaintext.reserve(text.size());
    for (const auto& row : original)
        plaintext.append(row.begin(), row.end());

    return plaintext;
}
