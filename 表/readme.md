##### 表

表数据结构通常有两种实现

- 数组实现

  数组实现需要先估计数组的大小

  插入、删除、查找操作花费线性时间

  返回第K个值花费常数时间

- 链表实现

  链表实现通常采用头节点（哑节点）这可以避免删除第一个节点的尴尬

  插入、删除操作花费常数时间

  查找操作花费线性时间

  返回第K个值花费O(K)时间

##### 链表实现

```c
/*链表的实现
 * 采用头节点
**/
#include <stdio.h>
#include <stdlib.h>    //for calloc free 
#ifndef _LinkList_H
#define _LinkList_H
struct node;
typedef struct node* List;
List CreatList();   //创建链表
void DeleteList(List L);  //删除链表
int IsEmpty(List L);    //判断链表是否为空
int LenList(List L);    //链表节点个数（不含头节点）
void Insert(int x,int pos,List L);  //插入节点
void Append(int x,List L);          //增加节点
void DeleteNode(int pos,List L);    //删除节点
#endif

typedef struct node
{
    int data;
    struct node* next;
}Node;

List CreatList()
{
    List head =(List)calloc(1,sizeof(Node));
    return head;
}
void DeleteList(List L)
{
    Node* P = L;
    Node* Tmp;
    while(P != NULL)
    {
        Tmp = P->next;
        free(P);
        P = Tmp;
    }
}

int IsEmpty(List L)
{
    return L->next == NULL;
}

int LenList(List L)  
{
    int len = 0;
    Node* P = L->next;
    while(P != NULL)
        {
            len++;
            P = P->next;
        }
    return len;
}

void Insert(int x,int pos,List L)  //在位置pos后插入节点，值为x,若pos==0这表示插入为头节点
{
    if(pos > LenList(L) || pos < 0)
    {
        printf("Node don't exit!\n");
        exit(1);
    }
    Node* P = L;
    for(int i=0;i < pos;i++)
        P = P->next;
    Node* tmp =(Node*)malloc(sizeof(Node));
    if(tmp == NULL)
        printf("Fatal error,out of space!\n");
    tmp->data = x;
    tmp->next = P->next;
    P->next = tmp;
}

void Append(int x,List L)  //在链表最后加入节点，值为x
{
    Node* P = L;
    while(P->next != NULL)
        P = P->next;
    Node* tmp =(Node*)malloc(sizeof(Node));
    if(tmp == NULL)
        printf("Fatal error,out of space!\n");
    tmp->data = x;
    tmp->next = NULL;
    P->next = tmp;
}

void DeleteNode(int pos,List L) //删除第pos个节点
{
    if(pos > LenList(L))
    {
        printf("Node don't exit!\n'");
        exit(1);
    }
    Node* P = L;
    for(int i=1;i<pos;i++)
        P = P->next;
    Node* tmp = P->next;
    P->next = tmp->next;
    free(tmp);
}

int main()
{
    List L = CreatList();
    printf("%d\n",IsEmpty(L));
    Append(10,L);
    Insert(11,0,L);
    Append(12,L);
    printf("%d\n",LenList(L));
    Node* P = L->next;
    while(P != NULL)
    {
        printf("%d ",P->data);
        P = P->next;
    }
    DeleteNode(2,L);
    P = L->next;
    while(P != NULL)
    {
        printf("%d ",P->data);
        P = P->next;
    }
    DeleteList(L);
}
```

