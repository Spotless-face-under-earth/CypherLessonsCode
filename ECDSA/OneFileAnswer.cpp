#include <iostream>
using namespace std;


auto GetEuler(auto n)//求欧拉函数
{
    auto result = n;
    //合数的欧拉函数需要分解质因数，这里只考虑质数的情况
    while(n%2==0){
       result = result / 2;
       std::cout<<"Non-Prime"<<std::endl;
       return result;
    }
    result = result - 1;
    return result;
}
auto SquarePow(auto a,auto b,auto m)//快速幂
{
    
    auto ans = 1;
    while(b)
    {
        if(b&1) ans = (ans * a) % m;
        a = (a * a ) % m;
        b >>= 1;
    }
    return ans;
}
inline int edgcd(int a,int b,int &x,int& y){
    if(!b){
        x=1,y=0;
        return a;
    }
    int d = edgcd(b,a%b,x,y);
    int yy =y;
    y= x-a/b*y;
    x = yy;
    return d;
}


auto ModReverse(auto a,auto m)//求逆元
{
    while(a < 0) a += m; 
    //assert(gcd(a, m) == 1);
    int r, q, s1 = 1, s2 = 0, s3, t1 = 0, t2 = 1, t3 = 1, mt = m;
    while(1){
        r = m % a; q = m / a;
        if(!r) break;
        m = a; a = r;
        s3 = s1 - q * s2;
        t3 = t1 - q * t2;
        s1 = s2; s2 = s3;
        t1 = t2; t2 = t3;
    }
    while(t3 < 0) t3 += mt;
    return t3 % mt;

}



class Node{
public:
    int x, y;
friend class Elliptic;
    Node(int x, int y);
    Node();
    friend ostream& operator<<(ostream &out, const Node &node);     // 重载输出
    friend istream& operator>>(istream &in, Node &node);     // 重载输入
};  // 椭圆曲线中的点

class Elliptic{
    int a, b, p;    // y^2 = x^3 + ax + b mod p
    int order;      // 群的阶
public:
    Elliptic(int a, int b, int p);
    Elliptic(){};
    Node minus(Node p);   // 求点的减法
    Node add(Node p1, Node p2);
    Node mul(Node p, int k);
    int getOrder();
    friend ostream& operator<<(ostream &out, const Elliptic &elliptic);     // 重载输出
    friend istream& operator>>(istream &in, Elliptic &elliptic);            // 重载输入
};

static Node UnLimitNode(-1, -1); // 无穷远点

/**
 * Node 类的构造函数、输入输出重载
 */

Node::Node(int x, int y){
    this->x = x;
    this->y = y;
}
Node::Node(){
    x = 0;
    y = 0;
}
ostream& operator<<(ostream &out, const Node &node) {
    out << "输出点：(" << node.x << ", " << node.y << ")";
    return out;
}
istream& operator>>(istream &in, Node &node) {
    // cout << "请输入点的 x 和 y 坐标：";
    in >> node.x >> node.y; // 依次读取 x 和 y
    return in; // 返回流对象，支持链式操作
}

/**
 * Elliptic 类的构造函数、加减乘法、输入输出重载
 */

Elliptic::Elliptic(int a, int b, int p){
    this->a = a;
    this->b = b;
    this->p = p;
}
Node Elliptic::minus(Node p){
    return Node(p.x, -p.y);
}
Node Elliptic::add(Node p1, Node p2){
    if(p1.x == -1 || p1.y == -1) return p2;
    if(p2.x == -1 || p2.y == -1) return p1;
    if(p1.x == p2.x && p1.y == p - p2.y) return UnLimitNode;
    int k;  // 斜率
    if(p1.x == p2.x && p1.y == p2.y){
        k = (3 * p1.x * p1.x + a) * ModReverse(2 * p1.y, p);
    }else{
        k = (p2.y - p1.y) * ModReverse(p2.x - p1.x, p);
    }
    int x = (k * k - p1.x - p2.x) % p;
    int y = (k * (p1.x - x) - p1.y) % p;
    if(x < 0) x += p;
    if(y < 0) y += p;

    return Node(x, y);
}
Node Elliptic::mul(Node p, int k){
    Node res = UnLimitNode;
    while(k){
        if(k & 1) res = add(res, p);
        p = add(p, p);
        k >>= 1;
    }
    return res;
}
int Elliptic::getOrder(){
    int count=0;
    for(int i=0;i<p;i++){
        for(int j=0;j<p;j++){
            if((j*j)%p==((i*i*i+a*i+b)%p)){
                count++;
            }
        }
    }
    count++;
    return count;
}
ostream& operator<<(ostream &out, const Elliptic &elliptic) {
    out << "输入椭圆曲线: y^2 = x^3 + " << elliptic.a << "x + " << elliptic.b << " mod " << elliptic.p;
    return out;
}
istream& operator>>(istream &in, Elliptic &elliptic) {
    in >> elliptic.a >> elliptic.b >> elliptic.p ;      // 依次读取 
    return in; // 返回流对象，支持链式操作
}


// #ifndef ECDSA_H
// #define ECDSA_H
// #pragma once
// #include "ECC.h"
class ECDSA{
    Elliptic ecc;       // 椭圆曲线 y^2 = x^3 + ax + b mod p
    long long m, x, k;  // m: 私钥，x: 消息，k: 随机数
    Node A;             // 基点
    long long r, s;      // 签名
public:
    auto Hash(auto x);  // 哈希函数
    ECDSA(Elliptic& ecc, long long m, long long x, long long k, Node& A);
    bool sign();
    bool verify();
    friend ostream& operator<<(ostream &out, const ECDSA &ecdsa);     // 重载输入
    friend istream& operator>>(istream &in, ECDSA &ecdsa);     // 重载输出
};
// #endif

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
bool ECDSA::sign(){
    bool success = true;
    Node B = ecc.mul(A, k);
    r = B.x % 13;
    if(r == 0){
        success = false;
        return success;
    }
    auto hash_x = Hash(x);
    long long inv_k = ModReverse(k, ecc.getOrder());
    s =( inv_k * (hash_x + (r * m) % ecc.getOrder()) )% ecc.getOrder();
    if(s == 0){
        success = false;
        return success;
    }
    return success;
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
    out<<ecdsa.r <<" "<< ecdsa.s;
    return out;
}
istream& operator>>(istream &in, ECDSA &ecdsa){
    cout << "请输入签名 r 和 s:";
    in >> ecdsa.r >> ecdsa.s;
    return in;
}



int main(){
    Elliptic ecc(1, 6, 11);
    Node A;
    cin >> A;
    long long m,x,k;
    cin >> m >> x >> k;
    ECDSA ecdsa(ecc, m, x, k, A);
    if( ecdsa.sign()==true){
       //cout<<"签名成功"<<endl;
        cout << ecdsa;
        return 0;
    }
    else{
        cin>>k;
        ecdsa = ECDSA(ecc,m,x,k,A);
        ecdsa.sign();
        cout<<ecdsa;
        return 0;
    }
   
}