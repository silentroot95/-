/*
 *散列，分离链接法
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef _Hash_H
#define _Hash_H
struct node;
typedef struct node *Node;
struct hashtable;
typedef int ElementType;
typedef struct hashtable *HashTable;
HashTable Initialize(int size);  //散列表初始化
void FreeTable(HashTable H);     //释放散列表
void Insert(HashTable H, ElementType x);  //插入值x
Node Find(HashTable H, ElementType x);   //查找值
int Hash(ElementType x, int size);      //Hash函数
void Delete(HashTable H, ElementType x);  //删除值
//ElementType Retrieve(Node n);
#endif
struct node
{
	ElementType data;
	Node next;
};
struct hashtable
{
	int size;
	Node *list;
};
static int IsPrime(int a) //判断a是否是素数
{
	int mid = (int)sqrt(a) + 1;
	for (int i = 2;i<mid;i++)
		if (a%i == 0)
			return 0;
	return 1;
}
static int NextPrime(int a) //大于a的第一个素数，a是素数取a
{
	while (!IsPrime(a))
		a++;
	return a;
}
HashTable Initialize(int size)
{
	HashTable H = (HashTable)malloc(sizeof(struct hashtable));
	if (H == NULL)
		printf("Fatal error out of space!\n");
	H->size = NextPrime(size);
	H->list = (Node*)calloc(H->size, sizeof(Node));//list是指向node结构体指针的指针，用calloc申请内存，内存直接初始化为NULL
	if (H->list == NULL)                          //省去了循环初始化NULL指针的麻烦,这样初始化的链表无头节点
		printf("Fatal error out of space!\n");   
	/* for(int i=0;i<H->size;i++)
	{ //初始化有头节点的链表，要使用下面的循环初始化
	H->list[i] = (Node)malloc(sizeof(struct node));
	if(H->list[i] == NULL)
	printf("Fatal error,out of space!\n");
	else
	H->list[i]->next = NULL;
	} */
	return H;
}
static void FreeList(Node first) //释放链表
{
	Node tmp;
	Node n = first;
	while (n != NULL)
	{
		tmp = n->next;
		free(n);
		n = tmp;
	}
}
void FreeTable(HashTable H)  //释放Hash表，调用FreeList释放链表
{
	for (int i = 0;i<H->size;i++)
		FreeList(H->list[i]);
	free(H);
}
Node Find(HashTable H, ElementType x)//查找x
{
	Node N = H->list[Hash(x, H->size)];
	while (N != NULL && N->data != x)
		N = N->next;
	return N;
}
void Insert(HashTable H, ElementType x)  //插入x
{
	Node pos = Find(H, x);  //先查找x是否存在
	if (pos == NULL)
	{
		Node new = (Node)malloc(sizeof(struct node));
		if (new == NULL)
			printf("out of space!");
		else
		{
			Node index = H->list[Hash(x, H->size)];  //index为链表第一个节点，链表无头节点
			H->list[Hash(x,H->size)] = new;         //链表的头部插入，new成为第一个节点，
			new->data = x;
			new->next = index;
		}
	}
    else
        printf("%d has already existed\n",x);
}
int Hash(ElementType x, int size)  //这是一个示例，应该采用一个好的Hash函数
{
	return x % size;
}
void Travel(HashTable H)  //输出散列表
{
    Node index;
    for(int i=0;i<H->size;++i)
    {
        printf("Index %d: ",i);
        index = H->list[i]; //index 是链表的第一个节点，链表无头节点
        while(index)
        {
            printf("%d ",index->data);
            index = index->next;
        }
        printf("\n");
    }
}
void Delete(HashTable H,ElementType x)
{
    Node pos = Find(H,x);
    if(pos)
    {
        Node first = H->list[Hash(x,H->size)];
        if(first == pos)  //如果删除第一个节点，操作有点不一样
        {
            H->list[Hash(x,H->size)] = first->next;
            free(first);
        }
        else
        {
            while(first->next != pos)
                first = first->next;
            first->next = pos->next;
            free(pos);
        }
    }
    else
        printf("%d is not in the hash table\n",x);
}
int main()
{
	HashTable H = Initialize(8);
    for(int i=0;i<11;++i)
        Insert(H,i*i);
    Travel(H);
    Delete(H,100);
    Travel(H);
	FreeTable(H);
	return 0;
}

