#ifndef ECDSA_H
#define ECDSA_H
#pragma once
#include "ECC.h"
class ECDSA{
    Elliptic ecc;       // 椭圆曲线 y^2 = x^3 + ax + b mod p
    long long m, x, k;  // m: 私钥，x: 消息，k: 随机数
    Node A;             // 基点
    long long r, s;      // 签名
public:
    auto Hash(auto x);  // 哈希函数
    ECDSA(Elliptic& ecc, long long m, long long x, long long k, Node& A);
    void sign();
    bool verify();
    friend ostream& operator<<(ostream &out, const ECDSA &ecdsa);     // 重载输入
    friend istream& operator>>(istream &in, ECDSA &ecdsa);     // 重载输出
};
#endif
