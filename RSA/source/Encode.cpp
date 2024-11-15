# include <iostream>
# include <deque>
# include <math.h>
using namespace std;

//RSA中的大素数p和q
int p,q;
//RSA中的模数n(p*q)和公钥e（指数）
long n;
long e;

//得到所有可能的质因数
deque<int> getPrimeFactor(int n)
{
	deque<int> allPossibleNum;
	int* p = new int[n];
	for (int i = 2;i < n;i++)
	{
		p[i] = i;
	}
	for (int i = 2;i < n;i++)
	{
		if (n % i == 0)
		{
			for (int k = 1;k <= n / i;k++)
			{
				p[i * k] = -1;
			}
		}
	}
	for (int i = 2;i < n;i++)
	{
		if (p[i] > 1)allPossibleNum.push_back(p[i]);
	}
	return allPossibleNum;
}

//得到所有小于它的素数
static deque<int> getPrimeNum(int n)
{
	deque<int> PrimeNums;
	PrimeNums.push_back(2);PrimeNums.push_back(3);
	for (int i = 5;i <=n;i++)
	{
		bool isPri = true;
		for (int j = 2;j <= sqrt(i);j++)
		{
			if (i % j == 0) { isPri = false;break; }
		}
		if (isPri)PrimeNums.push_back(i);
	}
	return PrimeNums;
}

//得到小于它的，所有不是素数的数
deque<int> getNotFactorPrime(int n)
{
	deque<int> notFactor;
	deque<int> isPrime = getPrimeNum(n);
	while (!isPrime.empty())
	{
		if (n % isPrime.front() == 0) { notFactor.push_back(isPrime.front());isPrime.pop_front(); }
		else isPrime.pop_front();
	}
	return notFactor;
}

//得到a的欧拉函数
int getEuroFunc(int a)
{	
	if (a == 2||a==1)return 1;
	else
	{
		if (getPrimeFactor(a).size()==static_cast<size_t>(a - 2))return a - 1;
		else
		{
			int EuroFunc = a;
			deque<int> tempNot = getNotFactorPrime(a);
			while (!tempNot.empty())
			{
				EuroFunc= (EuroFunc*(tempNot.front()-1)/ tempNot.front());
				tempNot.pop_front();
			}
			return EuroFunc;
		}
	;
	}
}

long ReverseEuclid(long a, long m) {
    long m0 = m, t0 = 0, t1 = 1;
    if (a == 0) return 0;  // 无逆元的情况
    while (a > 1) {
        int q = a / m;
        int temp = m;
        m = a % m;  // 更新 m 和 a
        a = temp;
        temp = t0;
        t0 = t1 - q * t0;  // 更新 t0 和 t1
        t1 = temp;
    }
    while (t1 < 0) t1 += m0;  // 确保逆元为正
    return t1;
}

//将十进制数转换为二进制，存入a数组中，返回值为a的长度
int toBio(int a[],int x)
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

long long squareMod(int a[],long b,long long m,int num)
{
	long long ans = 1;
	
	while (num >= 0)
	{
		if (a[num] == 1)
		{
			ans = (ans * ans) % m;
			ans = (ans * b) % m;
			if (ans < 0) {
				ans += m;
			}
		}
		else
		{
			ans = (ans * ans) % m;
			if (ans < 0) {
				ans += m;
			}
		}
		num--;
	}
	return ans;
	//cout<<" Answer:" << ans << endl;
}
long RSADecode(long long c)
{
	long long m;
	n=p*q;
	int biosD[100];
	//这里找到一个不确定数的欧拉函数的计算复杂度非常非常高，能确保p和q都为素数时，欧拉函数可以简单计算
	//long EuroN=getEuroFunc(n);

	long EuroN=(p-1)*(q-1);

	//cout<<"EuroN ="<<EuroN<<endl;//验证结果正确
	long d=ReverseEuclid(e,EuroN);
	//cout<<"d="<<d<<endl;//验证结果正确

	int numD=toBio(biosD,d);
	//cout<<"Num="<<tempNum<<endl;验证结果正确
	/*
	    这里格外要注意，只有在求公私钥时，才模n的欧拉函数；在加解密时，模n！！！
		!!!m=squareMod(biosD,c,EuroN,numD);!!!
	*/
	m=squareMod(biosD,c,n,numD);
	return m;
}
long RSAEncode(long m)
{
	long long c;
	n=p*q;
	int biosE[100];
	int numE=toBio(biosE,e);
	c=squareMod(biosE,m,n,numE);
	return c;
}
int main()
{
	long long c;//密文
    cin>>p>>q>>e>>c;
	cout<<RSADecode(c);
	return 0;
}