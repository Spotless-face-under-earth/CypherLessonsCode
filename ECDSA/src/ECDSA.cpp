# include"../include/ECDSA.h"
using namespace std;
ECDSA::ECDSA(Elliptic& ecc, long long m, long long x, long long k, Node& A){
    this->ecc = ecc;
    this->m = m;
    this->x = x;
    this->k = k;
    this->A = A;
}
auto ECDSA::Hash(auto x){
    return SquarePow(2, x, 13);
}
void ECDSA::sign(){
    Node B = ecc.mul(A, k);
    r = B.x % 13;
    auto hash_x = Hash(x);
    long long inv_k = ModReverse(k, ecc.getOrder());
    s = (inv_k * (hash_x + r * m)) % ecc.getOrder();
}
bool ECDSA::verify(){
    auto hash_x = Hash(x);
    long long inv_s = ModReverse(s, ecc.getOrder());
    long long u1 = (hash_x * inv_s) % ecc.getOrder();
    long long u2 = (r * inv_s) % ecc.getOrder();
    Node B = ecc.add(ecc.mul(A, u1), ecc.mul(ecc.minus(A), u2));
    return r == B.x % ecc.getOrder();
}
ostream& operator<<(ostream &out, const ECDSA &ecdsa){
    out << "签名：(" << ecdsa.r << ", " << ecdsa.s << ")";
    return out;
}
istream& operator>>(istream &in, ECDSA &ecdsa){
    cout << "请输入签名 r 和 s:";
    in >> ecdsa.r >> ecdsa.s;
    return in;
}

