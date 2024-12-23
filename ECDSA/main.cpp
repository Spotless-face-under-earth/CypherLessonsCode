# include "include/ECDSA.h"
using namespace std;
int main(){
    Elliptic ecc(1, 6, 11);
    Node A;
    cin >> A;
    long long m,x,k;
    cin >> m >> x >> k;
    ECDSA ecdsa(ecc, m, x, k, A);
    ecdsa.sign();
    cout << ecdsa;
    return 0;
}