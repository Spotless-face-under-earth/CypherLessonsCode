# pragma once
# include <iostream>
# include <math.h>
# include <deque>
using namespace std;
deque<int> getPrimeFactor(int n);
static deque<int> getPrimeNum(int n);
deque<int> getNotFactorPrime(int n);
int getEuroFunc(int a);

//欧几里得算法求逆元，source1为要求逆元的数，source2为模数
int ReverseEuclid(int source1, int source2);
//将十进制数转换为二进制数,方便求模时的计算
int toBio(int a[],int x);
//二进制求模
int squareMod(int a[],int b,long m,int num);
