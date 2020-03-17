/*栈的数组实现
**/
#include <stdio.h>
#include <stdlib.h> 
#ifndef _STACK_H
#define _STACK_H
struct stack;
typedef struct stack* Stack;
Stack CreatStack(int maxelement);  //创建栈
void DeleteStack(Stack S);       //删除栈
void MakeEmpty(Stack S);        //栈置空
int IsFull(Stack S);            //是否满栈
int IsEmpty(Stack S);           //是否空栈
void Push(int x,Stack S);       //入栈
int Pop(Stack S);               //栈顶元素出栈
#endif

struct stack
{
    int Capacity;          //栈容量
    int TopOfStack;       //栈顶
    int* Array;           
};

Stack CreatStack(int maxelements)
{
    Stack S =(Stack)malloc(sizeof(struct stack));
    if(S == NULL)
        printf("Fatal error,out of space!\n");
    S->Array =(int*)malloc(sizeof(int)*maxelements);
    if(S->Array == NULL)
        printf("Fatal error,out of space!\n");
    S->Capacity = maxelements;
    S->TopOfStack = -1;       //空栈栈顶指向-1
    return S;
}
void DeleteStack(Stack S)
{
    if(S!=NULL)
    {
        free(S->Array);
        free(S);
    }
}
void MakeEmpty(Stack S)
{
    S->TopOfStack = -1;  
}
int IsEmpty(Stack S)
{
    return S->TopOfStack == -1;
}
int IsFull(Stack S)
{
    return S->TopOfStack == S->Capacity-1;
}
void Push(int x,Stack S)
{
    if(IsFull(S))
        printf("Stack is full!\n");
    else
        S->Array[++S->TopOfStack] = x;
}
int Pop(Stack S)
{
    if(IsEmpty(S))
    {
        printf("Stack is empty!\n");
        return 0;
    }
    else
        return S->Array[S->TopOfStack--]; 
}
int main()
{
    Stack S = CreatStack(5);
    printf("%d\n",IsEmpty(S));
    Push(10,S);
    Push(12,S);
    printf("%d\n",Pop(S));
    printf("%d\n",Pop(S));
    return 0;
}



