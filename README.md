# Homomorphic Encryption

This repository provide a simple API for homomorphic encryption.

Homomorphic encryption is still a subject of research today, and no system that is both secure and efficient has yet been found. Furthermore, random number generation is done via the `rand()` function, which is notoriously unsuitable for cryptographic use. For these reasons, this library should not be used in production.

## Usage

### C

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
    gcc -Ofast -Wall -o build/bit_encryption.exe tests/bit_encryption.c src/include/homom/**.c src/include/pol/**.c -Isrc/include/homom -Isrc/include/pol
    ```

If one wants to use the library in a projet, they must include the `src/include` in their project tree, as well as including `homomorph.h` in their header file.

```c
#include "homomorph.h"

int main(void) {
    HomomContext ctx = {0};
    
    homomorph_init(d, dp, delta, tau, &ctx);

    // ctx can be used to encrypt
}
```

### Python

Python file is prertty straight-forward :
```py
from homom.py import encrypt, decrypt, cle_secrete, cle_publique

# Ready!
```

## Architecture

```bash
homomorph
├───python
├───src
│   └───include
│       ├───homom
│       └───pol
└───tests
```

The C library is located inside of `src` while the Python module is located inside of `python`. `tests` provides examples as well as tests for C the library.

## System

### Definition

The system is defined by 4 parameters :
$d, d', \delta < d, \tau \in \mathbb{N}$

A secret key $S$ is a randomly generated polynomial of degree $d$ in $\mathbb{Z}/2\mathbb{Z}_{d}[X]$.

A public key $T$ is $\tau$-list of polynomials in $\mathbb{Z}/2\mathbb{Z}_{d+d'}[X]$. These polynomials are generated as follows :

- Generate two random polynomials :
    - $Q_i \in \mathbb{Z}/2\mathbb{Z}_{d'}[X]$
    - $R_i \in \mathbb{Z}/2\mathbb{Z}_{\delta}[X]$
- $T_i$ is the sum $SQ_i + XR_i$

So that $T = (SQ_i + XR_i)_{1 \leq i \leq \tau}$

### Cipher

#### Encryption
Encryption of bit $x$ is done as follows :

- Generate $\mathcal{U} \in \mathcal{P}([1..\tau])$
- Encrypted polynomial is $C = (\sum_{i\in\mathcal{U}} T_i) + x$

$\mathcal{U}$ is used in order to protect against bruteforce. Indeed, if the sum was made over $[1..\tau]$, a malicious person could compute the cipher of $0$ and $1$ and easily compare them with the desired cipher. With $\mathcal{U}$ in the way, the number of possibilities is now $2^\tau$.

#### Decryption
Decryption of a cipher $C$ is done as follows :

- Compute $R$ the quotient of the euclidean division of $C$ by $S$
- $x$ is $R$ evaluated at $0$

This is why $\delta$ is under the condition $\delta < d$. Indeed, we recall that $C = \sum_{i\in\mathcal{U}} (SQ_i + XR_i) + x$, where $R_i$ has a degree of at most $\delta$, and $Q_i$ of at most $d'$. Thus, $R$ is exactly $(\sum_{i\in\mathcal{U}} XR_i) + x$, which gives $x$ when evaluated at $0$.

### Properties

This system is partially homomorphic, which means that it is not homomorphic with every operations.
However, one can prove that it is homomorphic with every [boolean function](https://en.wikipedia.org/wiki/Boolean_function#:~:text=In%20mathematics%2C%20a%20Boolean%20function,function\)%2C%20used%20in%20logic.) of degree less or equal than $\frac{d}{\delta}$.
