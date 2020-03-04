##### 堆

对需要满足的两个性质

1. 堆序性，父节点的值小于（或大于）子节点的值。
2. 堆是一棵完全二叉树，除最底层外，其他层全部填满，最底层从左到右依次填充。

堆，常被用来实现优先队列，堆的实现不用使用链表，简单的数组即可实现（因为堆是完全二叉树）。

##### 堆的常用操作

1. Initialize，初始化例程创建一个空堆，需要指定堆的容量.
2. Insert,插入一个新值，并维持堆的性质，时间复杂度O(logN).
3. PopMin,弹出并删除堆顶元素，时间复杂度O(1).
4. BuildHeap，用一个数组生成堆，采用自下向上方式建立堆，时间复杂度O(N).另一种创建堆的方法是，建立一个空堆然后逐个插入，这是自上而下的建堆方式，时间复杂度O(NlogN).
##### 自下向上建堆时间复杂度O(N)的证明：

举一个简单的例子，原始堆如下图，依次需要调整的元素为3、4、5，图中小字标处：

<img src=".\pic\0.png" style="zoom:50%;" />

调整3后（虚线显示调整路径），堆如下：

<img src=".\pic\1.png" style="zoom:35%;" />

调整4后，堆如下:

<img src=".\pic\2.png" style="zoom:35%;" />

调整5后，堆如下：

<img src=".\pic\3.png" style="zoom:35%;" />

可以看出，调整的次数就是虚线条数之和，虚线条数的上界为每个节点的高度之和。设堆为满树，高度为h，则第i层高度为$h-i$，根为第0层，第i层节点数为$2^i$ ,故  
$$
   \begin{eqnarray}
S&=&\sum_{i=0}^{h}2^i(h-i)\\
   &=&h+2(h-1)+4(h-2)+\cdots+2^{h-1}
   \end{eqnarray}
$$

$$
2S=2h+4(h-1)+8(h-2)+\cdots+2^h
$$

两式相减可得
$$
S=-h+2+4+8+\cdots+2^{h-1}+2^h=2^{h+1}-h-2
$$
堆为满二叉树时,节点的个数$N=2^{h+1}-1$，可见$S=O(N)$。

##### 具体实现

源代码如下

```c
/*
*堆，数组实现，无头标记，即数组下标从0开始，带有头标记的要多申请一个内存单元。
*/
#include <stdio.h>
#include <stdlib.h>
#ifndef _Heap_H
#define _Heap_H
struct heap;
typedef struct heap *Heap;
typedef int ElementType;
Heap Initialize(int capacity);       //创建空堆，容量为capacity
void FreeHeap(Heap H);              //释放堆
void Insert(Heap H, ElementType x); //插入值x
ElementType PopMin(Heap H);        //弹出并删除最小元
int IsEmpty(Heap H);               //判断堆是否满
int IsFull(Heap H);               //判断堆是否空
Heap BuildHeap(ElementType *a, int size,int capacity);   //用一个数组创建堆，数组大小size,堆容量capacity，采用自下向上方式建堆
void PrintHeap(Heap H);          //显印堆的数据
#endif
struct heap
{
	int capacity;  //堆的容量
	int size;     //堆的大小
	ElementType *datas;  //堆中元素
};
Heap Initialize(int capacity)  
{
	Heap H = (Heap)malloc(sizeof(struct heap));   //申请堆
	if (H == NULL)
		printf("Fatal error,out of space\n");
    //申请堆中数组，无头标记，即下标从0开始
	H->datas = (ElementType*)calloc(capacity, sizeof(ElementType)); 
	if (H->datas == NULL)
		printf("H->datas is NULL, out of space\n");
	H->capacity = capacity;
	H->size = 0;
	return H;
}
void FreeHeap(Heap H)  
{
	free(H->datas);
	free(H);
}
int IsFull(Heap H)   
{
	return H->size == H->capacity;
}
int IsEmpty(Heap H)
{
	return H->size == 0;
}
void Insert(Heap H, ElementType x)
{
	int i, parent = 0;
	if (IsFull(H))
	{
		printf("Heap is Full\n");
		return;
	}
	if (H->size == 0)
		H->datas[H->size++] = x; //当堆为空时，同时size自增1
	else
	{    //size自增1 i=0结束循环，因为下标为0时是最小值，无父节点
		for (i = H->size++;i != 0 && H->datas[(i - 1) / 2]>x;i = parent)  
		{
            //无论i为左儿子还是右儿子，父节点计算都一样，因为parent声明为整型，parent只保留整数位
			parent = (i - 1) / 2;  
			H->datas[i] = H->datas[parent];
		}
		H->datas[i] = x;
	}
}
ElementType PopMin(Heap H)
{
	int i, child = 0;
	if (IsEmpty(H))
	{
		printf("Heap is empty\n");
		return 0;
	}
	ElementType last = H->datas[--H->size]; //最后一个元素
	ElementType min = H->datas[0];         //最小元，第一个元素
	for (i = 0;2 * i + 1 <= H->size;i = child)
	{
		child = 2 * i + 1; //左儿子
        //如果有两个儿子，取最小的儿子
		if (child != H->size && H->datas[child + 1] < H->datas[child]) 
			child++; //右儿子
		if (last > H->datas[child])
		{
			H->datas[i] = H->datas[child]; //交换父节点与子节点
		}
		else
			break;
	}
	H->datas[i] = last;
	return min;
}
void PrintHeap(Heap H)
{
	for (int i = 0;i<H->size;++i)
		printf("%d ", H->datas[i]);
	printf("\n");
}
Heap BuildHeap(ElementType *a, int size, int capacity)
{
	int tmp, child,parent;
	int mid = (size - 2) / 2;
	Heap H = (Heap)malloc(sizeof(struct heap));
	if (H == NULL)
		printf("Fatal error,out of space\n");
    //这里重新申请一块内存有点浪费，可以直接将调整后的数组a赋给堆
    //这样带来的问题是，释放堆时，由于a是一个数组，可能不是由malloc申请的内存
    //那样free(H->datas)不是合法的操作
	H->datas = (ElementType*)calloc(capacity, sizeof(ElementType)); 
	if (H->datas == NULL)                                           
		printf("H->datas is NULL, out of space\n");                 
	H->capacity = capacity;
	H->size = size;
	for (int i = mid;i >= 0;--i)  //自下向上方式建堆，调整那些有儿子的节点
	{
        parent = i;  //此节点
		child = 2 * i + 1;  //子节点
		while (child < size)
		{
			if (child + 1 < size && a[child + 1] <a[child])   //如果右节点较小
				child++;  //右节点
			if (a[child] < a[parent])
			{
				tmp = a[parent];
				a[parent] = a[child];
				a[child] = tmp;
			}
            parent = child; //将child变为父节点，循环对其下子节点进行操作
			child = 2 * child + 1;
		}
	}
	for (int i = 0;i<size;++i)  //将a中的值复制到堆中，这个做法显得愚蠢，但它不影响堆的释放
		H->datas[i] = a[i];
	return H;
}

int main()
{
	int a[] = { 4,11,10,17,8,20,2,19,1,3 };
	Heap H = BuildHeap(a, 10, 20);
	PrintHeap(H);
	Insert(H, 7);
	PrintHeap(H);
	int min = PopMin(H);
	PrintHeap(H);
	FreeHeap(H);
	return 0;
}
```

