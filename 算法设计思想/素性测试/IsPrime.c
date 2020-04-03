#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
typedef unsigned _int64 UI64; //这是本机所能表达的最大整数，无符号64位整数,最大为2^64-1
int Prime(int N)
{
    int i=2;
    int end = sqrt(N)+1;
    for(i=2;i<end;++i)
        if(N%i==0)
            return 0;
    return 1;
}

UI64 Mod(UI64 A,UI64 i,UI64 N) //A是一个随机选择的底数，i是指数，N是要测试的数
{
    UI64 x,y;
    if(i==0)                    //递归的最后，指数为0时返回1
        return 1;
    x = Mod(A,i/2,N);    
    if(x==0)
        return 0;
    y = (x*x) % N;
    if(y==1 && x!=1 && x!=N-1)
        return 0;
    if(i%2 != 0)                //如果指数是奇数，再乘以一个底数A
        y = (A*y)%N;
    return y;
}
int IsPrime(UI64 N)
{
    int random = rand()%(N-3)+2;    //选择2到N-2的随机z整数
    return Mod(random,N-1,N)==1;
}
int main()
{
    int a = IsPrime(55566677);
    printf("%d",a);
    return 0;
}

