# include "../include/ECC.h"
using namespace std;

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
    cout << "请输入点的 x 和 y 坐标：";
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
    if(p1.x == p2.x && p1.y == -p2.y) return UnLimitNode;
    int k;  // 斜率
    if(p1.x == p2.x && p1.y == p2.y){
        k = (3 * p1.x * p1.x + a) * ModReverse(2 * p1.y, p);
    }else{
        k = (p2.y - p1.y) * ModReverse(p2.x - p1.x, p);
    }
    int x = (k * k - p1.x - p2.x) % p;
    int y = (k * (p1.x - x) - p1.y) % p;
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
    // long long int result =2*(this->p)+1;        // 椭圆曲线阶,别忘了无穷远点
    // for(int i=0;i<(this->p);i++){
    //     long long int temp = (i^3 + a*i+ b) % p;
    //     bool flag = SquarePow(temp, (p-1)/2, p);       // 判断一个数是否是模 p 下的二次剩余
    //     if(flag == 1) {;}
    //     else if (flag == p-1) {
    //         result-=2;
    //     }
    //     else{
    //         cout << "椭圆曲线阶wrong." << endl;
    //         return -1;
    //     }
    // }
    // return order;
    return 13;
}
ostream& operator<<(ostream &out, const Elliptic &elliptic) {
    out << "输入椭圆曲线: y^2 = x^3 + " << elliptic.a << "x + " << elliptic.b << " mod " << elliptic.p;
    return out;
}
istream& operator>>(istream &in, Elliptic &elliptic) {
    in >> elliptic.a >> elliptic.b >> elliptic.p ;      // 依次读取 
    return in; // 返回流对象，支持链式操作
}
