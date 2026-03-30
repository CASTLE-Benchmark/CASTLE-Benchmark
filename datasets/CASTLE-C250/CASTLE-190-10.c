#include <stdio.h>
#include <math.h>

long long power_mod(long long base, long long exp, long long mod) {
    /* Constant-time modular exponentiation (Montgomery ladder style) to avoid bit-dependent branches
       Preserves signature and returns (base^exp) % mod. */
    if (mod <= 0 || exp < 0) {
        return 0;
    }

    unsigned long long m = (unsigned long long) mod;
    long long base_mod_ll = base % mod;
    if (base_mod_ll < 0) base_mod_ll += mod;
    unsigned long long b = (unsigned long long) base_mod_ll;

    unsigned long long r0 = 1 % m;
    unsigned long long r1 = b % m;
    unsigned long long ue = (unsigned long long) exp;
    int bits = sizeof(ue) * 8;

    for (int i = bits - 1; i >= 0; --i) {
        unsigned long long bit = (ue >> i) & 1ULL;
        // Compute both candidate values unconditionally to avoid branching on secret bits
        unsigned long long t0 = (unsigned long long)((__int128)r0 * r0 % m);
        unsigned long long t1 = (unsigned long long)((__int128)r0 * r1 % m);
        unsigned long long t2 = (unsigned long long)((__int128)r1 * r1 % m);
        unsigned long long mask = 0ULL - bit; // PRECOGS_FIX: create mask from secret bit for constant-time select

        // Select new_r0 = bit ? t1 : t0 ; new_r1 = bit ? t2 : t1  (constant-time using mask)
        unsigned long long new_r0 = (t1 & mask) | (t0 & ~mask);
        unsigned long long new_r1 = (t2 & mask) | (t1 & ~mask);

        r0 = new_r0;
        r1 = new_r1;
    }

    return (long long)r0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long power_mod(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int main() {
    // Settings
    long long prime = 2147483647; // PRECOGS_FIX: use a secure prime number
    long long base = 5;   // generator

    // Private keys (generated securely)
    srand(time(NULL)); // PRECOGS_FIX: seed random number generator
    long long alice_private = rand() % (prime - 2) + 1;  // Alice (secret)
    long long bob_private = rand() % (prime - 2) + 1;   // Bob (secret)

    long long alice_public = power_mod(base, alice_private, prime);
    printf("Alice's public key: %lld\n", alice_public);

    long long bob_public = power_mod(base, bob_private, prime);
    printf("Bob's public key: %lld\n", bob_public);

    long long alice_shared_secret = power_mod(bob_public, alice_private, prime);
    printf("Alice's shared secret: %lld\n", alice_shared_secret);

    long long bob_shared_secret = power_mod(alice_public, bob_private, prime);
    printf("Bob's shared secret: %lld\n", bob_shared_secret);

    if (alice_shared_secret == bob_shared_secret) {
        printf("Key exchange successful! Shared secret: %lld\n", alice_shared_secret);
    } else {
        printf("Key exchange failed.\n");
    }

    return 0;
}