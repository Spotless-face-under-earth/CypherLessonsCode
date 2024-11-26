#include <iostream>
#include <cmath>
using namespace std;

long long int Euler(long long int p, long long int q) {
    return (p-1)*(q-1);
}

long long int gcd(long long int a, long long int b) {
    while (b != 0) {
        long long int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long long int ModInverse(long long int e, long long int phi) {
    long long int m0 = phi, t, q;
    long long int x0 = 0,x1 = 1;

    while (e > 1) {
        q = e / phi;
        t = phi;
        phi = e % phi, e = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0)
        x1 += m0;
    return x1;
}

long long int pow_mod(long long int a, long long int n, long long int m) {
    __int128 res = 1;        
    __int128 base = a % m;  

    while (n) {
        if (n & 1) {
            res = (res * base) % m;  
        }
        base = (base * base) % m;     
        n >>= 1;                     
    }

    return res; 
}

int main() {
    long long int p, q, e, c;
    cin >> p >> q >> e >> c;

    long long int n = p * q;
    long long int phi_n = Euler(p,q);
    long long int d = ModInverse(e, phi_n);
    long long int m = pow_mod(c, d, n);
    cout << m ;

    return 0;
}