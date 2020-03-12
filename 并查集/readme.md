##### 并查集

并查集是一种简单有趣的数据结构，并查集是关于一系列集合的操作，这些集合可以理解为树（不一定是二叉树）

每个集合都有一个根，查找一个节点，就返回这个集合的根，合并两个集合，就让一个集合的根指向另一个集合的根。并查集的实际实现采用数组，数组中的元素为父节点索引，对于根节点数组元素为包含树大小的信息。

并查集的常见操作

- 并(Union)

  求并操作可以按大小求并，按高度求并，这里采用按大小求并。

- 查(Find)

  查找操作采用路径压缩。

```c
/*并查集数据结构
 * Union操作，按大小求并
 * Find操作，采用路径压缩
**/
#include <stdio.h>
#include <string.h>
#ifndef _SetUnion_H
#define _SetUnion_H
void IniSet(int* set,int size);
int Find(int* set,int x);
void SizeUnion(int* set,int root1,int root2);
void RankUnion(int* set,int root1,int root2);
#endif

void IniSet(int* set,int size)  //初始化并查集
{
    memset(set,0xff,size*sizeof(int));  //根数组元素为-1
}

int Find(int* set,int x)
{
    if(set[x]<0)  //根元素
        return x;
    else
        return set[x] = Find(set,set[x]); //路径压缩
}

void SizeUnion(int* set,int root1,int root2)
{
    if(set[root1]<=set[root2])  
    {
        set[root1] += set[root2];  //按大小求并
        set[root2] = root1;   
    }
    else
        SizeUnion(set,root2,root1);
}
void RankUnion(int* set,int root1,int root2)  //按秩（高度）求并，这里的高度是一个估算高度（秩）,不是真实高度
{
    if(set[root2]<set[root1]) //root2高度更高，因为根数组元素为高度的相反数 
        set[root1] = root2;
    else
    {
        if(set[root1]==set[root2])  //高度相等
            set[root1]--;      //root1高度增1，
        set[root2] = root1;    //root2根为root1
    }
}
void PrintSet(int *set,int size)
{
    for(int i=0;i<size;++i)
        printf("%d ",set[i]);
    printf("\n");
}
int main()
{
    
    int set[9]={0};
    IniSet(set,9);
    PrintSet(set,9);
    RankUnion(set,1,2);
    SizeUnion(set,3,4);
    PrintSet(set,9);
    int root2 = Find(set,2);
    int root1 = Find(set,3);
    PrintSet(set,9);
    RankUnion(set,root1,root2);
    PrintSet(set,9);
    return 0;
}
```

##### 复杂度分析

空间复杂度O(n)。

时间复杂度相对复杂，求并操作是简单的，其时间复杂度为O(1)。

查找操作，**单独使用路径压缩或单独使用按秩求并**，其时间复杂度为$O(logN)$，

同时使用路径压缩和**按秩求并**，M次合并与查找时间复杂度为$O(M\alpha(N))$，其中$\alpha$为Ackerman函数的反函数，在很大的范围内这个函数的值可以认为不大于5，所以并查集每次操作的平均时间复杂度可以近似看作为O(1)，

对于同时使用路径压缩和**按大小求并**的时间复杂度可以达到和上面相同的界。