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
    gcc -o main.exe src/main.c src/homomorph.c src/include/*.c -Isrc/include -msse2
    ```
    Make sure SSE2 are supported by your CPU. Otherwise don't user -msse2
