# ifndef ECC_H
# define ECC_H
#include <iostream>
#include "BasicCalculate.h"
using namespace std;

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

# endif