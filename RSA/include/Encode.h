# pragma once
# include <iostream>
# include <deque>
# include <math.h>
using namespace std;

//RSA中的大素数p和q
inline int p,q;
//RSA中的模数n(p*q)和公钥e（指数）
inline long int n;
inline int e;

//RSA加密 
long RSAEncode(long m);

//RSA解密 
long RSADecode(long c);