# CipherLab

CipherLab is a C++17/JUCE desktop application for a BIS703 Information and Network Security assignment. It compares three classical/educational encryption techniques on the same normalized plaintext:

1. **Simple substitution** — a fixed permutation of A–Z.
2. **Double columnar transposition** — two successive columnar transpositions using two keywords.
3. **One-Time Pad (OTP)** — letter arithmetic modulo 26 with a freshly generated random key.

## Comparison rules

For a consistent comparison, all input is normalized to uppercase A–Z. Spaces, punctuation, digits, and other characters are removed before encryption. All three methods therefore operate on the same letter sequence.

- Simple substitution key: exactly 26 unique letters, mapping plaintext A through Z to the corresponding key letter.
- Double transposition: two comma-separated alphabetic keywords, e.g. `ZEBRA,CIPHER`. The implementation writes the message row-wise into a ragged rectangle and reads columns in alphabetical key order; it applies this operation twice and decrypts in reverse order.
- OTP: a fresh key of the same length as the normalized plaintext is generated automatically. The key is shown in the UI to support the demonstration; treat it as secret outside a classroom demo and never reuse it. A true OTP requires a uniformly random key at least as long as the message, secure key distribution, secrecy, and no key reuse. This educational letter-based implementation is not a general-purpose secure messaging system.

The UI reports ciphertext length, measured encryption time, and a round-trip check. Microsecond timings are only illustrative; they are not a rigorous performance or security benchmark.

## Build with Visual Studio Build Tools and VS Code

Prerequisites:
- CMake 3.22+
- Visual Studio Build Tools 2022 with the **Desktop development with C++** workload
- MSVC x64 tools and a Windows 10/11 SDK
- JUCE checked out in `JUCE/` (already configured as a submodule in this project)

Open **Developer PowerShell for VS 2022**, then:

```powershell
cd "C:\Coding Stuff\CipherLab"
git submodule update --init --recursive
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
```

Launch the app:

```powershell
.\build\Debug\CipherLab.exe
```

Depending on the generator and JUCE version, the executable may be under a target-specific build subdirectory. Search for it if needed:

```powershell
Get-ChildItem .\build -Filter CipherLab.exe -Recurse
```

## Run tests

```powershell
cmake --build build --config Debug --target CipherLabTests
ctest --test-dir build -C Debug --output-on-failure
```

## Important note about compiler generators

CMake caches the compiler and generator in the build directory. If you previously configured `build` with GCC or a different generator, delete that build directory before configuring with the Visual Studio generator. Do not delete the `JUCE` submodule.

## Academic scope

The program is an educational demonstration, not a secure cryptographic product. Simple substitution and transposition ciphers are not secure by modern standards. The OTP only achieves perfect secrecy when all OTP requirements are met; this demo's generated key is displayed for classroom inspection.