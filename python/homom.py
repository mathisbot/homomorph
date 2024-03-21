from numpy import polynomial as nppol
from random import randint

# Paramètres
N = 16  # Nombre de bits
d = 6  # Degré de la clé secrète
dp = 3  # Degré des éléments aléatoires
delta = 2  # Degré des éléments aléatoires
tau = 5  # Nombre de clé publiques


# Utilitaires

def monome(d):  # Renvoie le polynôme X^d
    return nppol.Polynomial([0 for _ in range(d-1)]+[1])

def polynome_aleatoire(d):  # Renvoie un polynôme aléatoire de degré au plus d
    coef = []
    for _ in range(d):
        coef.append(randint(0,1))
    return nppol.Polynomial(coef)

def polynome_constant(x):  # Renvoie le polynôme constant x
    return nppol.Polynomial([x])

def redmod(P, m):  # Renvoie le polynôme dont les coefficients sont les reste des div. eucl. de ceux de P par m
    return nppol.Polynomial([i%m for i in P.convert().coef])


# Génération de clé

def cle_secrete(d):  # Renvoie une clé secrète
    return monome(d) + polynome_aleatoire(d-1)

def cle_publique(S, dp, tau, delta):  # Renvoie une clé publique
    P = []
    for _ in range(tau):
        p1 = polynome_aleatoire(dp)*S
        p2 = polynome_aleatoire(delta)*[0,1]
        P.append(redmod(p1+p2, 2))
    return P


# Chiffrement de bits

def partie(tau):  # Renvoie une partie aléatoire de [|0;tau-1|]
    U = []
    for i in range(tau):
        if randint(0, 1):
            U.append(i)
    return U

def encrypt_bit(x, P, U):  # Chiffre le bit x
    const = polynome_constant(x)
    somme = polynome_constant(0)
    for i in U:
        somme += P[i]
    return redmod(const+somme, 2)

def decrypt_bit(P, S):  # Déchiffre le bit chiffré par P
    return int((P%S)(0)%2)


# Chiffrement

def encrypt(x, P):  # Chiffre un entier x en une liste de chiffrés
    # On commence par écrire x sous forme d'une liste de bits
    l = [0]*N
    for n, i in enumerate(str(bin(x))[:1:-1]):
        l[-n-1] = int(i)
    # On chiffre chacun des bits
    c = []
    U = partie(tau)
    for e in l:
        c.append(encrypt_bit(e, P, U))
    return c

def decrypt(Pl, S):
    d = 0
    # On convertit de la base 2 à la base 10
    for i, p in enumerate(Pl):
        d += 2**(N-i-1) * (decrypt_bit(p,S))
    return d


# Portes

def OR(c1, c2):
    return redmod(c1+c2 + c1*c2, 2)


# Opérations 

def addition_bits(c1, c2, cin):
    # bit courant
    S = redmod(c1+c2+cin, 2)
    # retenue
    cout = OR((c1+c2)*cin, c1*c2)
    return S, cout

def addition(c1, c2):
    c = []
    cin = polynome_constant(0) # La première retenue est nulle
    # On additionne bit par bit : les bits de poids faibles sont à droite
    for i in range(N):  # Dépassement possible /!\
        s, cin = addition_bits(c1[-i-1], c2[-i-1], cin)
        c.append(s)
    return c[::-1]
