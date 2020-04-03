#include <stdio.h> 
#include <stdlib.h> 

int Fib(int n)
{
    if(n<=2)
        return 1;
    else
        return Fib(n-1)+Fib(n-2);
}
int book(int *list,int n)
{
    if(list[n] == 0)
        list[n] = book(list,n-1)+book(list,n-2);
    return list[n];
}
int Fib2(int n)
{
    int *list = (int*)calloc(n+1,sizeof(int));
    list[1] = list[2] = 1;
    return book(list,n);
}
int Fib3(int n)
{
    int prev = 0,curr = 1;
    int ans;
    if(n<=2)
        return 1;
    for(int i=1;i<n;i++)
    {
        ans = prev + curr;
        prev = curr;
        curr = ans;
    }
    return ans;
}
int main()
{
    int n = 10;
    int ans1,ans2;
    ans1 = Fib(n);
    ans2 = Fib3(n);
    printf("%d  %d",ans1,ans2);
    return 0;
}

