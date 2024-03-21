import time
import rsa
from matplotlib import pyplot as plt
from homom import *

def moy(t):
    if t:
        return sum(t)/len(t)
    return None

nb_op = [10**i for i in range(1, 2)]
nb_tests = 1
max_nb = 2**(N-1)-1

time_rsa_add = []
time_rsa_mult = []
time_homom_add = []
time_homom_mult = []
for N in nb_op:
    print(f"Tests avec {N} opérations")

    temps = []
    print("Début des tests RSA")
    for t in range(nb_tests):
        pubkey, privkey = rsa.newkeys(512)
        total = 0
        print(" > Test", t+1)
        for i in range(N):
            x = str(randint(0, max_nb)).encode()
            y = str(randint(0, max_nb)).encode()
            cx = rsa.encrypt(x, pubkey)
            cy = rsa.encrypt(y, pubkey)
            debut = time.perf_counter()
            dx = rsa.decrypt(cx, privkey)
            dy = rsa.decrypt(cy, privkey)
            c = rsa.encrypt(dx+dy, pubkey)
            fin = time.perf_counter()
            total += fin-debut
        temps.append(total)
    time_rsa_add.append(moy(temps))

    temps = []
    print("Début des tests Homomorphes")
    for t in range(nb_tests):
        s = cle_secrete(d)
        p = cle_publique(s, dp, tau, delta)
        total = 0
        print(" > Test", t+1)
        for i in range(N):
            x = randint(0, max_nb)
            y = randint(0, max_nb)
            cx = encrypt(x, p)
            cy = encrypt(y, p)
            debut = time.perf_counter()
            addition(cx, cy)
            fin = time.perf_counter()
            total += fin-debut
        temps.append(total)
    time_homom_add.append(moy(temps))

plt.title("Addition")
plt.xscale("log")
plt.scatter(nb_tests, time_rsa_add, label="RSA", color='red')
plt.scatter(nb_tests, time_homom_add, label="Homom", color='blue')
plt.legend()

plt.show()
