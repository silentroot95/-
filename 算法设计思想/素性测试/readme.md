##### 素性测试

素性测试即判断一个数N是不是素数，这很有用。一个常规的想法是对$[2,\sqrt{N}]$的所有数进行判断，如果找到一个数能整除N,那N就不是素数，否则N就是素数。我们的想法是对一个很大的数，能不能以更快地判断它是不是一个素数。这方面的算法也有较多，这里说明一下Miller-Rabin测试。

##### 常规算法

常规的循环测试，其实已经不错了。

```c
int IsPrime(int N)
{
    int i=2;
    int end = sqrt(N)+1;
    for(i=2;i<end;++i)
        if(N%i==0)
            return 0;
    return 1;
}
```

##### Miller-Rabin测试

Miller-Rabin测试不是一个百分百正确的方法，但它的出错概率非常低，而且我们完全可以控制误判的概率让它任意低。Miller-Rabin测试使用下面两个定理。定理1的证明稍微复杂，这里不介绍，可自行百度，定理2是显而易见的。

1. 费马小定理

   如果$P$是素数且$0<A<P$，那么$A^{P-1}\equiv1(mod\ P)$。

2. 定理

   如果$P$是素数且$0<X<P$,那么$X^2\equiv1(mod\ P)$仅有两个解$X=1,P-1$。

具体的测试步骤这里举一个例子：

要测试341是不是一个素数，随机选一个A，假设A取2，计算$2^{340}\ mod\ P$是否为1，如果不为1那肯定不是素数，如果是1此时还不能下结论，计算结果确实为1。如果341是一个素数，那么$2^{170} mod\ P$,只可能是1或340，计算结果确实为1，继续如上步骤，计算$2^{85}mod\ P=32$,此时就发现341不是素数。

对于一个大数N，可能进行几步后指数就为奇数了，此时指数除以2就变为了小数，不过这并不影响，整个算法的实现可以使用递归，当发现指数为奇数时在原来的基础上再乘一个底数A就行了。

此算法最大的问题就是要防止溢出。对于大数的测试，如1234567894987654321是一个素数，用下面的程序测试由于溢出出现误判，怎么解决溢出是一个很大的问题

```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
typedef unsigned _int64 UI64; //这是本机所能表达的最大整数，无符号64位整数,最大为2^64-1
UI64 Mod(UI64 A,UI64 i,UI64 N) //A是一个随机选择的底数，i是指数，N是要测试的数
{
    UI64 x,y;
    if(i==0)				//递归的最后，指数为0时返回1
        return 1;
    x = Mod(A,i/2,N);
    if(x==0)
        return 0;
    y = (x*x) % N;        //当N很大时，x可能很大，这一步相乘很容易溢出
    if(y==1 && x!=1 && x!=N-1)
        return 0;
    if(i%2 != 0)		//如果指数是奇数，再乘以一个底数A
        y = (A*y)%N;
    return y;
}
int IsPrime(UI64 N)
{
    int random = rand()%(N-3)+2;	//选择2到N-2的随机整数
    return Mod(random,N-1,N)==1;
}
```

为了解决溢出的问题，需要一个新的函数来求余，这里我们用到了一个基本的定理，它也容易理解。
$$
X\ mod\ N = P
$$
则:
$$
2X\ mod\ N=2P\ mod\ N
$$
根据上面的基本定理，我们做乘法时每次乘2，函数代码如下：

```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
typedef unsigned _int64 UI64; //这是本机所能表达的最大整数，无符号64位整数,最大为2^64-1
UI64 max = _UI64_MAX>>1;
UI64 mul_mod(UI64 X, UI64 Y, UI64 N) //计算（X*Y) mod N
{
	UI64 CZ1 = _UI64_MAX / X;
	UI64 res=0,tmp;
	if (Y <= CZ1)           //如果没有溢出，直接返回
		return (X*Y) % N;
	else 
	{
		while(Y)            //对乘数Y分解，每次Y除以2
        {
            if(Y&1)	//如果Y是奇数，先拿出一个X放到res中，即先Y-1使其变为偶数
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
        return res;  //最后Y肯定取到1，返回res
    }
}
UI64 Mod(UI64 A,UI64 i,UI64 N) //A是一个随机选择的底数，i是指数，N是要测试的数
{
    UI64 x,y;
    if(i==0)				//递归的最后，指数为0时返回1
        return 1;
    x = Mod(A,i/2,N);
    if(x==0)
        return 0;
    y = mul_mod(x,x,N);        //当N很大时，x可能很大，这一步相乘很容易溢出
    if(y==1 && x!=1 && x!=N-1)
        return 0;
    if(i%2 != 0)		//如果指数是奇数，再乘以一个底数A
        y = (A*y)%N;
    return y;
}
int IsPrime(UI64 N)
{
    int random = rand()%(N-3)+2;	//选择2到N-2的随机整数
    //int random[4] = {2,7,11,197}  //多选几个数测试
    return Mod(random,N-1,N)==1;
}
```

使用rand()函数生成随机数很糟糕，首次运行总是生成相同的随机数，使用srand()函数要好些，最好多选几个数测试，准确率更高。

