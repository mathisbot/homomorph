# Homomorphic Encryption

This repository provide a simple API for homomorphic encryption.

## Usage

1. Clone the repository:

    ```bash
    git clone https://github.com/mathisbot/homomorph.git
    ```

2. Navigate to the project directory:

    ```bash
    cd homomorph
    ```

3. Compile with
    ```
    gcc -Ofast -Wall -o build/bit_encryption.exe test/bit_encryption.c src/include/homom/**.c src/include/pol/**.c -Isrc/include/homom -Isrc/include/pol
    ```
    If SSE2 are supported by your CPU, add -msse2.

## Improvements

Store polynomials as array of 64-bit numbers to improve computation time.
This should divide computational time by 64.