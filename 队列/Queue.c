/*队列ADT
 *数组实现
**/
#include <stdio.h>
#include <stdlib.h> 
#ifndef _QUEUE_H
#define _QUEUE_H
struct queue;
typedef struct queue Queue;
Queue* CreatQueue(int max);  //创建队列
void DelQueue(Queue* Q);     //销毁队列
void MakeEmpty(Queue* Q);    //队列置空
int IsFull(Queue* Q);
int IsEmpty(Queue* Q);
void Enqueue(int v,Queue* Q); //入队
int Dequeue(Queue* Q);        //出队
#endif
struct queue{
    int Capacity;     //队列容量
    int size;        //队列大小
    int front;      //队头
    int rear;      //队尾
    int* list;
};
Queue* CreatQueue(int max)
{
    Queue* Q=(Queue*)calloc(1,sizeof(Queue));
    Q->list =(int*)calloc(max,sizeof(int));
    Q->Capacity = max;
    return Q;
}
void DelQueue(Queue* Q)
{
    free(Q);
}
void MakeEmpty(Queue* Q)
{
    Q->size = 0;
    Q->front = 1;
    Q->rear = 0;
}
int IsFull(Queue* Q)
{
    return Q->size == Q->Capacity;
}
int IsEmpty(Queue* Q)
{
    return Q->size ==0;
}
void Enqueue(int v,Queue* Q)
{
    if(IsFull(Q))
    {
        printf("Full Queue!");
    }
    else
    {
        Q->size++;
        if(++Q->rear == Q->Capacity)  //环形队列，如果队满，队尾返回索引0点
            Q->rear = 0;
        Q->list[Q->rear] = v;
    }
}
int Dequeue(Queue* Q)
{
    if(Q->size == 0)
    {
        printf("Empty Queue!");
        exit(EXIT_SUCCESS);
    }
    else
    {
        Q->size--;
        int res = Q->list[Q->front];
        if(++Q->front == Q->Capacity)//环形队列同上
            Q->front=0;
        return res;
    }
}

int main()
{
    int a;
    Queue* Q = CreatQueue(3);
    MakeEmpty(Q);
    Enqueue(5,Q);
    Enqueue(2,Q);
    a = Dequeue(Q);
    printf("%d\n",a);
    a = Dequeue(Q);
    printf("%d\n",a);
    a = Dequeue(Q);
    DelQueue(Q);
    return 0;
}
