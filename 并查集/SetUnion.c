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
