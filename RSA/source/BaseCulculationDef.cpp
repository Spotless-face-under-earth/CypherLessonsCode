# include "BaseCulculationDef.h"
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
	if (getPrimeFactor(a).size() == a - 2)return a - 1;
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

int ReverseEuclid(int source1, int source2)
{
	//bool sr1mores2 = false;
	long int t[100], s[100], q[100];
	int a = source1;
	int b = source2;
	s[0] = 1;s[1] = 0;
	t[0] = 0;t[1] = 1;
	int count = 1;
	if (source1 > source2)
	{
		//sr1mores2 = true;
		q[1] = a / b;
		source1 = source1 % source2;
	}
	else {
		source2 = source2 % source1;
		q[1] = b / a;
	}
	//int addCount = 0;
	while (source1 != 0 && source2 != 0)
	{
		//addCount++;
		//count++;
		while (source1 > source2 && source2 != 0)
		{
			count++;
			q[count] = source1 / source2;
			s[count] = s[count - 2] - q[count - 1] * s[count - 1];
			t[count] = t[count - 2] - q[count - 1] * t[count - 1];
			source1 = source1 % source2;
		}
		while (source1 <= source2 && source1 != 0)
		{
			count++;
			q[count] = source2 / source1;
			s[count] = s[count - 2] - q[count - 1] * s[count - 1];
			t[count] = t[count - 2] - q[count - 1] * t[count - 1];
			source2 = source2 % source1;
		}
	}
	//如果s或t小于0，加上模数
	if (s[count] < 0 || t[count] < 0)		
	{
		if (s[count] < 0)s[count] += b;
		else t[count] += a;
	}
	return s[count];
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

int squareMod(int a[],int b,int m,int num)
{
	long int ans = 1;
	
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