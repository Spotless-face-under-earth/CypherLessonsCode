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


auto ModReverse(auto a,auto b)//求逆元
{

    int x, y;


    int gcd = edgcd(a, b, x, y);


    if (gcd != 1) {


        printf("逆元不存在n");


        return -1;


    } else {


        // 确保结果为正数


        return (x % b + b) % b;


    }


}
    // auto x = 0,y = 0;
    // auto phi = m - 1;
    // auto result = SquarePow(a,phi-2,m);
    // if(result <= 0) {
    //     std::cout << "逆元wrong.\n";
    //     return -1;
    // }
    // return result;
        // while(a < 0) a += m; 
        // //assert(gcd(a, m) == 1);
        // int r, q, s1 = 1, s2 = 0, s3, t1 = 0, t2 = 1, t3 = 1, mt = m;
        // while(1){
        //     r = m % a; q = m / a;
        //     if(!r) break;
        //     m = a; a = r;
        //     s3 = s1 - q * s2;
        //     t3 = t1 - q * t2;
        //     s1 = s2; s2 = s3;
        //     t1 = t2; t2 = t3;
        // }
        // while(t3 < 0) t3 += mt;
        // return t3 % mt;
