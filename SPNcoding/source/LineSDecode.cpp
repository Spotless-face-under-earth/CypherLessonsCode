#include "../include/SPNCoding.h"
#include <iostream>
// int K[32]={0};
// int cyberWords[16]={0};
// int plainWords[16]={0};
using namespace std;
/*
    这里有个问题，我们都知道线性分析需要找到几个比特进行模拟，找比特时需要尽可能使得偏差的绝对值大，并且异或之后要能够消去某些中间变量。
    并且最最重要的一点，各个活动S盒的选取是相互独立的，只需要考虑P盒中的置换输出条件即可。
    但是选取合适的活动S盒是一个非常困难的事！！！
    在这里密钥解密的第4-8和12-16位密钥解密式按照电子工业出版社《密码学原理与实践第三版》P59页进行设计；第
*/
int ActiveSBox[4]={0*m+2,1*m+2,2*m+2,2*m+3};
int LineralRoundKey[8]={0};
// 待求解的4-8,12-16位候选密钥
int candidateKeysA[8][256] = {0};
// 待求解的0-4,8-12位候选密钥
int candidateKeysB[8][256] = {0};
int* InversegetRoundeKey(int T)//T为最开始使用的明密文对数
{
    // 声明一个 8x256 的数组
    int CandidateKeyNumA[256]={0};
    int CandidateKeyNumB[256]={0};

    // 填充数组
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 8; ++j) {
            candidateKeysA[j][i] = (i >> (7 - j)) & 1;  // 提取第 j 位的二进制位（从高到低）
            candidateKeysB[j][i] = (i >> (7 - j)) & 1;  // 提取第 j 位的二进制位（从高到低）
        }
    }

    //明文随机生成，密钥暂时固定（假装不知道）
    string Kstring="00111010100101001101011000111111";
    string Nstring="11011110001100111010010110110001";
    for(int i=0;i<32;i++)
    {
        K[i]=Nstring[i]-48;
    }
    for(int i=0;i<T;i++)
    {
        for(int j=0;j<16;j++)
        {
            plainWords[j]=rand()%2;
        }
        int* cyberWords=Coding(plainWords);
        for(int j=0;j<256;j++)
        {
            //保存密钥可能的4-8和12-16位
            int tempV[8];
            for(int k=0;k<8;k++)
            {
                if(k<4){
                    tempV[k]=candidateKeysA[k][j]^cyberWords[1*m+k];
                }
                else{
                    tempV[k]=candidateKeysA[k+4][j]^cyberWords[3*m+k];
                }
            }
            int inverseTempV[8];
            for(int k=0;k<8;k+=4)
            {
                int tempValue=0;
                int inverseValue=0;
                tempValue=tempV[k]+tempV[k+1]*2+tempV[k+2]*4+tempV[k+3]*8;
                inverseValue=inverseSmapping[tempValue];
                inverseTempV[k+3]=inverseValue/8;
                inverseValue=inverseValue%8;
                inverseTempV[k+2]=inverseValue/4;
                inverseValue=inverseValue%4;
                inverseTempV[k+1]=inverseValue/2;
                inverseValue=inverseValue%2;
                inverseTempV[k]=inverseValue;   
            }
            int inspectValue=0;
            inspectValue=inverseTempV[1]^inverseTempV[3]^inverseTempV[5]^inverseTempV[7]^plainWords[5]^plainWords[7]^plainWords[8];
            if(inspectValue==0)
            {
                CandidateKeyNumA[j]++;
            }
        }

    }
    int RedLine=0,ans=0;
    for(int i=0;i<256;i++)
    {
        if(abs(CandidateKeyNumA[i]-T/2)>=RedLine)
        {
            RedLine=abs(CandidateKeyNumA[i]-T/2);
            ans=i;
        }
    }
    cout<<"正确结果T:"<<CandidateKeyNumA[111]<<"******";//正确结果
    if(ans==0)
    {
        cout<<"No candidate key found"<<endl;
        return NULL;
    }
    cout<<"MAX Close to T is "<<RedLine<<endl;
    /*
    计算最后一轮轮密钥的第0-4位和8-12位
    */
    for(int i=0;i<T;i++)
    {
        for(int j=0;j<16;j++)
        {
            plainWords[j]=rand()%2;
        }
        int* cyberWords=Coding(plainWords);
        for(int j=0;j<256;j++)
        {
            //保存密钥可能的0-4和8-12位
            int tempV[16];
            for(int k=0;k<16;k++)
            {
                if(k<4||(k>=8&&k<12)){
                    tempV[k]=candidateKeysB[k][j]^cyberWords[k];
                }
                else{
                    tempV[k]=candidateKeysB[k][ans]^cyberWords[k];
                }
            }
            int inverseTempV[16];
            for(int k=0;k<16;k+=4)
            {
                int tempValue=0;
                int inverseValue=0;
                tempValue=tempV[k]+tempV[k+1]*2+tempV[k+2]*4+tempV[k+3]*8;
                inverseValue=inverseSmapping[tempValue];
                inverseTempV[k+3]=inverseValue/8;
                inverseValue=inverseValue%8;
                inverseTempV[k+2]=inverseValue/4;
                inverseValue=inverseValue%4;
                inverseTempV[k+1]=inverseValue/2;
                inverseValue=inverseValue%2;
                inverseTempV[k]=inverseValue;
            }
            int inspectValue1=0,inspectValue2=0;
            inspectValue1=inverseTempV[1]^inverseTempV[5]^inverseTempV[9]^inverseTempV[13]^plainWords[1]^plainWords[2]^plainWords[4];
            if(inspectValue1==0)
            {
                CandidateKeyNumB[j]++;
            }
            inspectValue2=inverseTempV[3]^inverseTempV[7]^inverseTempV[11]^inverseTempV[15]^plainWords[9]^plainWords[10]^plainWords[12];
            if(inspectValue2==0)
            {
                CandidateKeyNumB[j]++;
            }
        }

    }
    int RedLine2=0,ans2=0;
    for(int i=0;i<256;i++)
    {
        if(abs(CandidateKeyNumB[i]-T)>=RedLine2)
        {
            RedLine2=abs(CandidateKeyNumB[i]/2-T);
            ans2=i;
        }
    }
    // cout<<"正确结果T:"<<CandidateKeyNumB[203]/2<<"******";//正确结果
    // if(ans2==0)
    // {
    //     cout<<"No candidate key found"<<endl;
    //     return NULL;
    // }
    // cout<<"MAX Close to T is "<<RedLine2<<endl;
    /*
    合并两部分密钥
    */
    for (int i = 0; i < 16; i++) {
        if((i>=4&&i<8)){
            LineralRoundKey[i] = candidateKeysA[i-4][ans];
        }
        else if(i>=12&&i<16){
             LineralRoundKey[i] = candidateKeysA[i-8][ans];
        }
        else if(i<4){
            LineralRoundKey[i] = candidateKeysB[i][ans2];
        }
        else{
            LineralRoundKey[i] = candidateKeysB[i-4][ans2];
        }
    }
    return LineralRoundKey;
}
int main()
{
    //顺序不能反，因为InversegetRoundeKey(8000)函数中才对K进行初始化；
    int* candidateKey2=InversegetRoundeKey(8000);
    cout<<"Lineral inverse candidate key is:";
    for(int i=0;i<16;i++)
    {
        cout<<candidateKey2[i]<<" ";
    }
    cout<<endl;
    int* candidateKeyAns=getRoundeKey(5);
    cout<<"Correct candidate key is:";
    for(int i=0;i<16;i++)
    {
        cout<<candidateKeyAns[i]<<" ";
    }
    cout<<endl;
    // cout<<"Lineral inverse candidate key<2> is:";
    // for(int i=0;i<4;i++)
    // {
    //    cout<<candidateKey2[i]<<" ";
    // }
    // cout<<endl;
    // cout<<"Lineral inverse candidate key<4> is:";
    // for(int i=4;i<8;i++)
    // {
    //    cout<<candidateKey2[i]<<" ";
    // }
    // cout<<endl;
    // int* candidateKeyAns=getRoundeKey(5);
    // cout<<"Correct candidate key<2> is:";
    // for(int i=4;i<8;i++)
    // {
    //     cout<<candidateKeyAns[i]<<" ";
    // }
    // cout<<endl;
    // cout<<"Correct candidate key<4> is:";
    // for(int i=12;i<16;i++)
    // {
    //     cout<<candidateKeyAns[i]<<" ";
    // }
    // cout<<endl;
    // system("pause");
    // for (int i = 0; i < 256; ++i) {
    //     cout << "Combination " << i << ": ";
    //     for (int j = 0; j < 8; ++j) {
    //         cout << candidateKeys[j][i];
    //     }
    //     //std::cout << std::endl;
    // }
}
