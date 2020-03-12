##### 散列

散列是一种以常数时间执行插入、删除、查找的数据结构。但它不支持数据的排序信息。

##### 冲突解决

- 分离链接法
- 开放定址法 

本处采用分离链接法实现。

##### 散列函数

散列函数不是本文的重点，本文只关注散列数据结构的一些常用操作例程的实现。散列函数如下：

```c
int Hash(ElementType x, int size)  //这是一个示例，应该采用一个好的Hash函数
{
	return x % size;
}
```

##### 数据结构的声明

```c
#ifndef _Hash_H
#define _Hash_H
struct node;
typedef struct node *Node;
struct hashtable;
typedef int ElementType; //此处的ElementType理想情况下可以为任何类型，但此代码均是基于int型的测试
typedef struct hashtable *HashTable;
HashTable Initialize(int size);   //散列表初始化
void FreeTable(HashTable H);      //销毁散列表
void Insert(HashTable H, ElementType x);  //插入例程
Node Find(HashTable H, ElementType x);    //查找例程
int Hash(ElementType x, int size);        //散列函数
void Delete(HashTable H, ElementType x);  //删除例程
//ElementType Retrieve(Node n);
#endif
struct node  //链表节点
{
	ElementType data;
	Node next;
};
struct hashtable  //散列表
{
	int size;
	Node *list;
};
```

##### 初始化散列表

初始化例程先申请hashtable结构体的内存，再申请list链表域的内存。

链表域内存的申请有两种方式：

1. 链表无头节点，直接申请size个内存单元，每个内存单元存放Node型（指向结构体）的指针。
2. 链表有头节点，要循环申请内存并初始化，这样多次调用函数申请内存会降低程序的效率，因此本文采用第一种方式。

```c
HashTable Initialize(int size)
{
	HashTable H = (HashTable)malloc(sizeof(struct hashtable));
	if (H == NULL)
		printf("Fatal error out of space!\n");
	H->size = NextPrime(size);  //NextPrime函数取size的下一个素数
    //list是指向node结构体指针的指针，用calloc申请内存，内存直接初始化为NULL
    //省去了循环初始化NULL指针的麻烦，这样初始化的链表无头节点
	H->list = (Node*)calloc(H->size, sizeof(Node));
	if (H->list == NULL)                          
		printf("Fatal error out of space!\n");    
	/* for(int i=0;i<H->size;i++)  
	{         //初始化有头节点的链表，要使用下面的循环初始化
	H->list[i] = (Node)malloc(sizeof(struct node));
	if(H->list[i] == NULL)
	printf("Fatal error,out of space!\n");
	else
	H->list[i]->next = NULL;
	} */
	return H;
}
```

##### 销毁散列表

销毁散列表，先释放链表域内存，再释放散列表内存。代码如下：

```c
static void FreeList(Node first) //释放链表，有无头节点均可                            
{                               //函数声明为static类型表示只能在当前文件中调用
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
	static void FreeList(Node first);
	for (int i = 0;i<H->size;i++)
		FreeList(H->list[i]);
	free(H);
}
```

##### 查找值

查找函数相对简单，代码如下：

```c
Node Find(HashTable H, ElementType x)//查找x
{
	Node N = H->list[Hash(x, H->size)];
	while (N != NULL && N->data != x)
		N = N->next;
	return N;
}
```

##### 插入值

当插入一个值时应先判断这个值是否在表中存在，插入操作和链表的插入相同，这里采用头部插入。

```c
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
```

##### 删除值

删除时先查找值是否存在，注意删除第一个节点时，操作有点不一样。

```c
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
```

##### 全部源代码

```c
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
```

