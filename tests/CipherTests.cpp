#include "../source/ciphers/SimpleSubstitution.h"
#include "../source/ciphers/DoubleTransposition.h"
#include "../source/ciphers/OneTimePad.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
    int failures = 0;

    void check(bool condition, const std::string& name)
    {
        if (condition)
            std::cout << "[PASS] " << name << '\n';
        else
        {
            std::cerr << "[FAIL] " << name << '\n';
            ++failures;
        }
    }

    template <typename Function>
    void checkThrows(Function function, const std::string& name)
    {
        try
        {
            function();
            check(false, name);
        }
        catch (const std::exception&)
        {
            check(true, name);
        }
    }
}

int main()
{
    const std::string plaintext = "ATTACKATDAWN";

    SimpleSubstitution substitution;
    const std::string substitutionKey = "QWERTYUIOPASDFGHJKLZXCVBNM";
    const auto substitutionCipher = substitution.encrypt(plaintext, substitutionKey);
    check(substitution.decrypt(substitutionCipher, substitutionKey) == plaintext,
          "Simple substitution encrypt/decrypt round-trip");
    check(!SimpleSubstitution::isValidKey("AAAAAAAAAAAAAAAAAAAAAAAAAA"),
          "Reject duplicate substitution alphabet");
    check(SimpleSubstitution::normalize("Attack, at dawn! 123") == plaintext,
          "Normalize plaintext to uppercase A-Z");

    DoubleTransposition transposition;
    const auto transpositionCipher = transposition.encrypt(plaintext, "ZEBRA", "CIPHER");
    check(transposition.decrypt(transpositionCipher, "ZEBRA", "CIPHER") == plaintext,
          "Double transposition encrypt/decrypt round-trip");
    check(transposition.encrypt("A", "ZEBRA", "CIPHER").size() == 1,
          "Transposition handles short, ragged rectangles");
    checkThrows([&] { transposition.encrypt(plaintext, "", "CIPHER"); },
                "Reject empty transposition key");

    OneTimePad otp;
    const auto key = otp.generateKey(plaintext.size());
    check(key.size() == plaintext.size(), "OTP generates a message-length key");
    check(otp.decrypt(otp.encrypt(plaintext, key), key) == plaintext,
          "OTP encrypt/decrypt round-trip");
    checkThrows([&] { otp.encrypt(plaintext, "ABC"); }, "Reject OTP key with wrong length");

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
