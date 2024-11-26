# include <iostream>
# include <deque>
# include <math.h>
using namespace std;

//RSA中的大素数p和q
int p,q;
//RSA中的模数n(p*q)和公钥e（指数）
long long int n;
long long int e;

__int128 ReverseEuclid(__int128 a, __int128 m) {
	__int128 m0 = m, t0 = 0, t1 = 1;
	if (m == 1) return 0;  // 无逆元的情况
	while (a > 1) {
		__int128 q = a / m;
		__int128 temp = m;
		m = a % m;  // 更新 m 和 a
		a = temp;
		temp = t0;
		t0 = t1 - q * t0;  // 更新 t0 和 t1
		t1 = temp;
	}
	if (t1 < 0) t1 += m0;  // 确保逆元为正
	if (t1/m0>1) t1=t1%m0;
	return t1;
};

//将十进制数转换为二进制，存入a数组中，返回值为a的长度
int toBio(int a[],__int128 x)
{
	int num = 0;
	while(x != 0) 
		{
			a[num] = x % 2;//a是x的二进制数
			x = x / 2;
			//cout << a[num]<<"*";
			num++;
		}
	return num-1;
}
/*
	快速幂取模，这里使用一种比较新的方法。
	首先将幂指数转化为二进制，例如6=110，那么a^6=(a^2*a)^2.
	即除第一位之外，二进制位为0时是上次结果的平方，二进制位为1时是上次结果的平方再乘以a
*/
__int128 squareMod(int a[],__int128 b,__int128 m,int num)
{
	__int128 ans = 1;
	
	while (num >= 0)
	{
		if (a[num] == 1)
		{			
			ans = (ans * ans) % m;
			ans = (ans * (b%m)) % m;		
		}
		else
		{
			ans = (ans * ans) % m;	
		}
		num--;
	}
	return ans;
}


/*
	解密函数，返回解密后的明文
*/
__int128 RSADecode(long long c)
{
	__int128 m;
	n = static_cast<long long int>(p) * static_cast<long long int>(q);
	int biosD[100];
	
	//这里找到一个不确定数的欧拉函数的计算复杂度非常非常高，能确保p和q都为素数时，欧拉函数可以简单计算
	//long EuroN=getEuroFunc(n);

	__int128 EuroN=__int128(p-1)*__int128(q-1);

	__int128 d=ReverseEuclid(e,EuroN);

	int numD=toBio(biosD,d);
	/*
	    这里格外要注意，只有在求公私钥时，才模n的欧拉函数；在加解密时，模n！！！
		!!!m=squareMod(biosD,c,EuroN,numD);!!!
	*/
	m=squareMod(biosD,c,n,numD);
	return m;
}
/*
	加密函数，返回加密后的密文
*/
__int128 RSAEncode(long long m)
{
	long long c;
	//这里要做一步类型转换，否则乘法会先在int内计算，再转化为long long，导致会出现溢出
	n = static_cast<long long int>(p) * static_cast<long long int>(q);
	int biosE[100];
	__int128 numE=toBio(biosE,e);
	c=squareMod(biosE,m,n,numE);
	return c;
}
/*
	打印128位整数
*/
void printInt128(__int128 value) {
    if (value == 0) {
        std::cout << "0";
        return;
    }

    bool negative = value < 0;
    if (negative) {
        cout<<"Error::m is minus!"<<endl;
		return;
    }

    std::string result;
    while (value > 0) {
        result = char('0' + (value % 10)) + result;
        value /= 10;
    }

    std::cout << result;
}

int main()
{
	long long c;//密文
    cin>>p>>q>>e>>c;
	printInt128(RSADecode(c));
	//cout<<RSADecode(c);
	return 0;
}