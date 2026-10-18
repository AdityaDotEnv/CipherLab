# CipherLab — Classical Cipher Demonstration Tool

CipherLab is a desktop application developed in C++ using the JUCE framework for the **BIS703: Information and Network Security** course. It demonstrates three classical encryption techniques through a graphical interface, allowing users to encrypt plaintext, inspect ciphertext, decrypt results, and compare encryption execution times.

## Features

- **Simple Substitution Cipher** — substitutes each plaintext letter using a user-provided substitution alphabet.
- **Double Transposition Cipher** — rearranges plaintext characters using row and column permutations applied to a rectangular matrix.
- **One-Time Pad (OTP)** — performs letter-based encryption using modular arithmetic and a generated key.
- **Encryption and decryption** — supports round-trip verification for each implemented cipher.
- **Input normalization** — converts plaintext to uppercase and removes characters outside A–Z.
- **Execution timing** — displays encryption timing information in the GUI.
- **Automated tests** — verifies cipher operations, normalization, key validation, and round-trip correctness.

## Technology Stack

| Component | Technology |
|---|---|
| Programming language | C++ |
| GUI framework | JUCE |
| Build system | CMake |
| Compiler | Microsoft Visual C++ (MSVC) |
| IDE / terminal | Visual Studio 2022 Build Tools and PowerShell |
| Testing | C++ test executable integrated with CTest |

## Prerequisites

- Windows 10 or Windows 11
- Git
- CMake
- Visual Studio 2022 Build Tools with the **Desktop development with C++** workload
- A compatible Windows SDK
- The JUCE submodule initialized in the project directory

## Setup and Build

### 1. Clone the repository

```powershell
git clone <repository-url>
cd CipherLab
git submodule update --init --recursive
```

Replace `<repository-url>` with the actual Git repository URL.

If the project is already present locally and the JUCE directory is populated, skip the cloning step.

### 2. Configure the project

Open a PowerShell terminal with the Visual Studio C++ build environment available, navigate to the project directory, and run:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

### 3. Build the application

```powershell
cmake --build build --config Debug
```

### 4. Launch CipherLab

```powershell
.\build\CipherLab_artefacts\Debug\CipherLab.exe
```

### 5. Run the automated tests

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

Alternatively, run the test executable directly:

```powershell
.\build\Debug\CipherLabTests.exe
```

## Implemented Algorithms

### 1. Simple Substitution Cipher

Each plaintext letter is mapped to a corresponding ciphertext letter according to a substitution alphabet.

A valid substitution alphabet must contain all 26 letters of the English alphabet exactly once. Decryption uses the inverse mapping to recover the normalized plaintext.

**Purpose:** Demonstrates monoalphabetic substitution and reversible character mapping.

### 2. Double Transposition Cipher

The normalized plaintext is arranged into a rectangular matrix. A row permutation and a column permutation determine the order in which matrix elements are selected to produce the ciphertext.

The implementation accepts two comma-separated integer permutations, separated by a semicolon:

```text
Row permutation;Column permutation
```

For example:

```text
3,1,2;2,4,1,3
```

This specifies a three-row, four-column matrix. The row permutation contains three entries, and the column permutation contains four entries.

Permutation values are **1-based**. Each permutation must contain every integer from `1` through its length exactly once.

The normalized plaintext length must equal:

```text
Number of rows × Number of columns
```

For example, the permutation above requires exactly 12 normalized letters. A message containing 18 normalized letters would require a different compatible matrix, such as a 3 × 6 matrix.

The current implementation does not automatically pad messages or select matrix dimensions. Users must provide compatible permutations for the normalized plaintext length.

Decryption reconstructs the original matrix arrangement by reversing the row and column reordering.

**Purpose:** Demonstrates matrix-based transposition, permutation operations, and reversible character rearrangement.

### 3. One-Time Pad (OTP)

The implementation represents letters as values from 0 to 25. Encryption adds the plaintext and key values modulo 26:

`C = (P + K) mod 26`

Decryption subtracts the key modulo 26:

`P = (C - K + 26) mod 26`

The application generates a key matching the normalized message length. The generated key is displayed in the GUI and must be kept secret and never reused.

**Purpose:** Demonstrates modular arithmetic and the theoretical properties and practical requirements of a one-time pad.

## Input Handling

Plaintext is normalized to uppercase letters A–Z. Spaces, punctuation, digits, and other characters are removed before encryption.

For example:

```text
Input:      Attack, at dawn! 123
Normalized: ATTACKATDAWN
```

Consequently, decryption restores the normalized plaintext rather than the original spacing, punctuation, digits, or letter case.

### Double Transposition input requirements

The Double Transposition panel accepts row and column permutations separated by a semicolon.

Example for a 12-character normalized message:

```text
3,1,2;2,4,1,3
```

Example for an 18-character normalized message:

```text
3,1,2;2,4,1,6,3,5
```

Both permutations must be valid, and their lengths must multiply to the normalized message length.

## Testing

The automated test suite is intended to cover:

- Simple substitution encryption/decryption round trip
- Rejection of a substitution alphabet containing duplicate letters
- Plaintext normalization
- Double transposition encryption/decryption round trip
- Handling of short transposition inputs
- Rejection of invalid transposition permutations
- OTP key generation and key length
- OTP encryption/decryption round trip
- Rejection of an OTP key with the wrong length

Run the test suite using CTest as described in the build instructions.

**Note:** Confirm that all tests pass against the current implementation before treating the test suite as fully validated.

## Security Limitations

CipherLab is an educational demonstration, not a production cryptographic application.

- **Simple substitution** is vulnerable to frequency analysis and other classical cryptanalysis techniques.
- **Double transposition** is a classical cipher and does not provide modern cryptographic security.
- **One-Time Pad** provides perfect secrecy only when its key is uniformly random, independent of the plaintext, kept secret, at least as long as the message, and never reused. The application's generated key should not be treated as a security certification.
- **Input normalization** discards spaces, punctuation, digits, and letter case.
- **Execution timings** are intended for demonstration and are not rigorous cryptographic benchmarks.

Do not use this application to protect sensitive or real-world communications.
