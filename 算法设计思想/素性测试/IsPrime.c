#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef unsigned _int64 UI64; //这是本机所能表达的最大整数，无符号64位整数,最大为2^64-1
UI64 mul_mod(UI64 x, UI64 y, UI64 N);
UI64 max = _UI64_MAX>>1;
int Prime(UI64 N)
{
	UI64 i = 2;
	UI64 end = sqrt(N) + 1;
	for (i = 2;i<end;++i)
		if (N%i == 0)
			return 0;
	return 1;
}
UI64 Mod(UI64 A, UI64 i, UI64 N) //A是一个随机选择的底数，i是指数，N是要测试的数
{
	UI64 x, y;
	if (i == 0)                    //递归的最后，指数为0时返回1
		return 1;
	x = Mod(A, i / 2, N);
	if (x == 0)
		return 0;
	y = mul_mod(x,x,N);
	if (y == 1 && x != 1 && x != N - 1)
		return 0;
	if (i % 2 != 0)                //如果指数是奇数，再乘以一个底数A
		y = (A*y) % N;
	return y;
}
UI64 mul_mod(UI64 X, UI64 Y, UI64 N)
{
	UI64 CZ1 = _UI64_MAX / X;
	UI64 res=0,tmp;
	if (Y <= CZ1)           //如果没有溢出，直接返回
		return (X*Y) % N;
	else 
	{
		while(Y)            //对乘数Y分解，每次Y除以2
        {
            if(Y&1)         //如果Y是奇数，先拿出一个X放到res中，即先Y-1使其变为偶数，最后Y肯定取到1，返回res
            {
                if(res>_UI64_MAX - X) //对于X特别大，相加溢出的情况，使用res-(N-X)避免溢出
                {
                    tmp = N-X;
                    res = res-tmp;
                    
                }
                else
                {
                    res = (res+X)%N;  //拿出一个X放到res中
                }
            }
            if(X>max)       //对于X特别大，相加溢出，用X-(N-X)计算余数
            {
                tmp = N-X;
                X = X - tmp;
            }
            else
            {
                X = (X<<1)%N;   //左移1即2X
            }
            Y >>=1;    //Y除以2
        }
        return res;
    }
}
int IsPrime(UI64 N)
{
	int random[4] = {2,3,43,125};    //选择2到N-2的随机z整数
    for(int i=0;i<4;++i)
        if(Mod(random[i],N-1,N) != 1)
            return 0;
	return 1;
}
int main()
{
    for(UI64 i = 100000;i<1000000;++i)
    {
        if(Prime(i))
            printf("%I64u\n",i);
    }
	return 0;
}


