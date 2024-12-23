# include <iostream>
# include <vector>
# include <algorithm>
# include <random>
using namespace std;  

//a为底，d为指数，n为模数
//把d看作二进制数。并按照乘法的嵌套规则进行计算。
auto power =[](auto a, auto d, auto n) {
    __int128 result = 1;
    a = a % n;
    while (d > 0) {
        if (d % 2 == 1) {
            result = (result * a) % n;
        }
        d = d >> 1;
        a = (a * a) % n;
    }
    return result;
};

auto millerTest = [&](auto d, auto n, auto s) {
    __int128 a = rand() % (n-1);
    __int128 x = power(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for(int i = 0; i <= s-2; i++) {
        x = power(x, 2, n);
        if (x == n - 1) return true;
        if(x == 1) return false;
    }
    return false;
};

bool MillerRabin(__int128 n)
{
    if (n < 2) return false;
    if (n==2)return true;
    if (n % 2 == 0) return false;

    __int128 d = n - 1;
    //s为2的指数;d经过迭代后为最后的奇数
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }
    for (int i = 0; i < 4; i++) {
        if (!millerTest(d, n, s)) return false;
    }

    return true;

}
int main(){
    long long m;
    // __int128 n;
    cin >> m;
    // n=m & 0x0000000000000000FFFFFFFFFFFFFFFF;
    if(MillerRabin(m)){
        cout << "Yes" ;
    }else{
        cout << "No";
    }
    //cout << "Yes" << endl;
    return 0;
}