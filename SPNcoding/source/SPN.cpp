#include<iostream>
#include<string>
#include<SPNCoding.h>
using namespace std;
// int K[32]={0};
// int plainWords[16]={0};
// int cyberWords[16]={0};
int paiS(int input)
{
    auto it = Smapping.find(input);  
    if (it != Smapping.end()) {  
        return it->second; // 返回找到的值  
    } else {  
        // 处理未找到的情况，可以返回一个特殊值或抛出异常  
        return -1; // 示例：返回-1表示未找到  
    }  
}
int paiP(int input)
{
    auto it = Pmapping.find(input);  
    if (it != Pmapping.end()) {  
        return it->second; // 返回找到的值  
    } else {  
        // 处理未找到的情况，可以返回一个特殊值或抛出异常  
        return -1; // 示例：返回-1表示未找到  
    }  
}
int* getRoundeKey(int r)
{
    int startPoint=(4*r-4)%32;//
    int* tempK=new int[32];
    int i=0,j=startPoint;
    while(j<=31)
    {
        tempK[i]=K[j];
        i++;j++;
    }
    j=j-32;
    while(j<startPoint)
    {
        tempK[i]=K[j];
        i++;j++;
    }
    return tempK;
}
int* Coding(int plainWords[16])
{
    //以下为具体密码计算过程
    int roundNum=1;
    int tempW[16],tempU[16],tempV[16];
    int i=0;
    while(i<16)
    {
        tempW[i]=plainWords[i];
        i++;
    }
    while(roundNum<=Nr)
    {
        int *KPointer=getRoundeKey(roundNum);
        int j=0;
        /*
        得到轮密钥，进行异或
        */
        while(j<16)
        {
            tempU[j]=tempW[j]^*(KPointer+j);
            j++;
        }
        /*
        每四个数为一组，进行S盒的非线性置换
        */
        int jj=0;
        while(jj<m)
        {
            int temp4UValue=8*tempU[jj*l]+4*tempU[jj*l+1]+2*tempU[jj*l+2]+tempU[jj*l+3];
            int temp4UKey=paiS(temp4UValue);
            tempV[jj*l]=temp4UKey/8;;temp4UKey=temp4UKey-tempV[jj*l]*8;
            tempV[jj*l+1]=temp4UKey/4;temp4UKey=temp4UKey-tempV[jj*l+1]*4;
            tempV[jj*l+2]=temp4UKey/2;temp4UKey=temp4UKey-tempV[jj*l+2]*2;
            tempV[jj*l+3]=temp4UKey;
            jj++;
        }
        if(roundNum!=Nr)//非最后一轮时，进行P盒置换
        {
            /*
            每个数进行P盒的线性替换
            */
            int jjj=0;
            while(jjj<16)
            {
                int position=paiP(jjj+1)-1;
                tempW[jjj]=tempV[position];
                jjj++;
            }
            
        }
        if(roundNum==Nr)//最后一轮时，还需要进行一次轮密钥异或
        {
            int j=0;
            int *KNextPointer=getRoundeKey(roundNum+1);
            while(j<16)
            {   
                tempW[j]=tempV[j]^*(KNextPointer+j);
                j++;
            }
            delete[] KNextPointer;
        }
        delete[] KPointer;
        roundNum++;
    }
    for(int i=0;i<16;i++)
    {
        cyberWords[i]=tempW[i];
    }
    return cyberWords;
}

// int main()
// {
//     int plainWordsNum=0;
//     while(plainWordsNum<=15)
//     {
//         char c=getchar();
//         if(c=='0'||c=='1')
//         {
//             plainWords[plainWordsNum]=c-48;
//             plainWordsNum++;
//         }
//         //if (plainWordsNum%4==0)getchar();//读掉空格       
//     }
//     //getchar();//读取换行符
//     int kNum=0;
//     while(kNum<=31)
//     {   
//         char c=getchar();
//         if(c=='0'||c=='1')
//         {
//             K[kNum]=c-48;
//             kNum++;
//         }
//         //if (kNum%4==0)getchar();//读掉空格   
//     }
//     int* ans=Coding(plainWords);
//     for(int i=0;i<16;i++)
//     {
//         cout<<cyberWords[i];
//     }
//     cout<<endl;
// }