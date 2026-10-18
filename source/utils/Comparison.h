#pragma once
#include <cstdint>
#include <string>

// Small result object shared by the GUI. Timings are illustrative local measurements,
// not a security benchmark.
struct ComparisonResult
{
    std::string ciphertext;
    std::string key;
    std::int64_t encryptionMicroseconds = 0;
    bool roundTripPassed = false;
};
