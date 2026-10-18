#pragma once

#include <string>

class DoubleTransposition
{
public:
    // Applies row and column permutations to a rectangular row-major matrix.
    // Permutations are comma-separated, 1-based indices (e.g. "3,1,2").
    // The normalized text length must equal row-count x column-count.
    std::string encrypt(const std::string& plaintext,
                        const std::string& rowPermutation,
                        const std::string& columnPermutation) const;
    std::string decrypt(const std::string& ciphertext,
                        const std::string& rowPermutation,
                        const std::string& columnPermutation) const;

    static bool isValidPermutation(const std::string& permutation);
};
