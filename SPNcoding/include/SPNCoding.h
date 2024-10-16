#include<map>
#include<iostream>
#include<string>
using namespace std;
//inline 定义全局常量
inline map<int ,int>Smapping ={{0,14},{1,4},{2,13},{3,1},
                        {4,2},{5,15},{6,11},{7,8},{8,3},{9,10},
                        {10,6},{11,12},{12,5},{13,9},{14,0},{15,7}};

inline map<int, int> inverseSmapping = {{14, 0}, {4, 1}, {13, 2}, {1, 3},
                                 {2, 4}, {15, 5}, {11, 6}, {8, 7}, {3, 8}, {10, 9},
                                 {6, 10}, {12, 11}, {5, 12}, {9, 13}, {0, 14}, {7, 15}};


inline map<int ,int>Pmapping ={{1, 1}, {2, 5}, {3, 9}, {4, 13},  
                        {5, 2}, {6, 6}, {7, 10}, {8, 14},  
                        {9, 3}, {10, 7}, {11, 11}, {12, 15},  
                        {13, 4}, {14, 8}, {15, 12}, {16, 16}};
inline int m=4,l=4,Nr=4;
//extern 声明全局变量
inline int K[32]={0};
inline int plainWords[16]={0};
inline int cyberWords[16]={0};
int paiS(int input);
int paiP(int input);
int* getRoundeKey(int r);
int* Coding(int plainWords[16]);
int* InverseCoding(int T);


