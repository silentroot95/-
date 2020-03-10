#include <stdio.h>
#include <time.h>
void PrintList(int a[],int n)  //输出数组，用于测试
{
    for(int i=0;i<n;++i)
        printf("%d ",a[i]);
    printf("\n");
}

void InsertionSort(int a[],int n)  //插入排序
{
    int j,p;
    int tmp;
    for(p=1;p<n;++p)
    {
        tmp = a[p];  // 数组中的一个新值
        for(j=p;j>0 && a[j-1]>tmp;--j)   //倒序扫描前面的值，直到找到应该插入的位置
            a[j] = a[j-1];     
        a[j] = tmp;
    }
}

void ShellSort(int a[],int n)  //希尔排序
{
    int step,p,i,j,tmp;
    for(step=n/2;step>0;step/=2)  //step为每次插入排序的间距，最终要取到1，即最后一次执行的是插入排序，不过此时数组大部分已是有序的
    {
        for(p=step;p<n;p++)  //下面的例程和插入排序一样
        {
            tmp = a[p];
            for(i=p;i>=step && a[i-step]>tmp;i-=step)
                a[i] = a[i-step];
            a[i] = tmp;
        }
    }
}
void SelectionSort(int a[],int n) //直接选择排序
{
    int i,j,tmp,min;
    for(i = 0;i<n;i++)
    {
        min = i;      //最小元索引
        for(j=i;j<n;j++)
        {
            if(a[j]<a[min])
                min = j;   //更新最小元
        }
        tmp = a[min];  //最小元与a[i]替换
        a[min] = a[i];
        a[i] = tmp;
    }
}
static void Down(int a[],int n,int i)  //下滤例程，堆排序的主例程
{
    int child;
    int tmp;
    for(tmp = a[i];2*i+1<n;i=child)
    {
        child = 2*i+1;
        if(child+1<n && a[child+1]>a[child])  //如果有右儿子，且右儿子较大
            child++;                        //取右儿子
        if(tmp<a[child])
            a[i] = a[child];
        else
            break;
    }
    a[i] = tmp;
}
void HeapSort(int a[],int n)
{
    int i,tmp;
    for(i=n/2;i>=0;--i)
        Down(a,n,i);   //至下向上建堆过程
    for(i=n-1;i>0;i--) //堆排序
    {
        tmp = a[0];
        a[0] = a[i];
        a[i] = tmp;
        Down(a,i,0);
    }
}
void BubbleSort(int a[],int n) //冒泡排序
{
    int i,j,tmp;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n-1-i;j++)
            if(a[j]>a[j+1])
            {
                tmp = a[j];
                a[j] = a[j+1];
                a[j+1] = tmp;
            }
    }
}
static void Merge(int a[],int tmp[],int lpos,int rpos,int rend) 
{  //tmp为用来合并两个子序列的临时数组，lpos为左子序列左端索引，rpos为右子序列左端索引，rend为右子序列右端索引
    int i,lend,nums,tmppos; 
    lend = rpos-1;  //做子序列右端索引
    tmppos = lpos;  //开始时tmp的索引指向左子序列左端
    nums = rend-lpos+1;//序列总长度
    while(lpos <=lend && rpos <= rend)
    {
        if(a[lpos]<=a[rpos])  //依次比较左右子序列的值，把较小的值放入tmp，此处的<=保证排序的稳定性
            tmp[tmppos++] = a[lpos++];
        else
            tmp[tmppos++] = a[rpos++];
    }
    while(lpos <= lend)  //把剩余的元素拷贝到tmp中，这两个循环中只会执行一个
        tmp[tmppos++] = a[lpos++];
    while(rpos <= rend)
        tmp[tmppos++] = a[rpos++];
    for(i=0;i<nums;i++,rend--)  //把tmp拷贝回去
        a[rend] = tmp[rend];
}
static void MSort(int a[],int tmp[],int left,int right)
{
    int center;
    if(left<right)  //至少有两个元素，才能归并排序
    {
        center = (left+right)/2;
        MSort(a,tmp,left,center);
        MSort(a,tmp,center+1,right);
        Merge(a,tmp,left,center+1,right);
    }
}
void MergeSort(int a[],int n)  //归并排序主例程
{
    int *tmp; 
    tmp = (int*)malloc(n*sizeof(int));
    if(tmp != NULL)
    {
        MSort(a,tmp,0,n-1);
        free(tmp);
    }
    else
        printf("No space for tmp array\n");
}
static void swap(int *a,int *b) //交换数组中的两个值，使用指针更容易操作
{
    int tmp = *b;
    *b = *a;
    *a = tmp;
}
static int Median3(int a[],int left,int right)  //三分中值法求枢纽元
{ //取第一个元素，最后一个和中间元素的中值
    int center = (left+right)/2;  //中间元素索引
    if(a[center] < a[left])
        swap(&a[center],&a[left]);
    if(a[right] < a[left])
        swap(&a[right],&a[left]);
    if(a[right] <a[center])
        swap(&a[right],&a[center]);
    swap(&a[center],&a[right-1]);
    return a[right-1];  //由于最后一个元素大于枢纽元，枢纽元存在倒数第二个位置
}
#define cutoff 3  //对于小于3个的序列直接插入排序
void Qsort(int a[],int left,int right)
{
    int i,j;
    int mid;
    if(left+cutoff <= right)  //序列元素大于3个，快速排序
    {
        mid = Median3(a,left,right);
        i = left;
        j = right-1;
        for(;;)
        {
            while(a[++i] < mid){}  //跳过左侧小于枢纽元的值
            while(a[--j] > mid){}  //跳过右侧大于枢纽元的值
            if(i<j)
                swap(&a[i],&a[j]);  //交换左侧大于枢纽元的值 和 右侧小于枢纽元的值
            else
                break;
        }
        swap(&a[i],&a[right-1]);  //枢纽元归位
        Qsort(a,left,i-1);
        Qsort(a,i+1,right);
    }
    else  //序列元素小于3个，直接插入排序
        InsertionSort(a+left,right-left+1);
}
void QuickSort(int a[],int n) //快速排序主程序
{
    Qsort(a,0,n-1);
}
static int _max(int a[],int n) //返回数组最大值
{
    int res=a[0];
    for(int i=1;i<n;i++)
        if(a[i]>res)
            res = a[i];
    return res;
}
void BucketSort(int a[],int n)
{
    int i;
    int  buckets = _max(a,n)+1;  //桶的数量
    int *bucket = (int*)calloc(buckets,sizeof(int));
    int *tmp = (int*)malloc(sizeof(int)*n);  
    for(i=0;i<n;++i)
        bucket[a[i]]++;
    for(i=1;i<buckets;++i)
        bucket[i] += bucket[i-1];
    for(i=n-1;i>=0;--i) //逆序扫描数组，保证排序稳定性
        tmp[--bucket[a[i]]] = a[i]; 
    for(i=0;i<n;++i)
        a[i] = tmp[i];
    free(tmp);
    free(bucket);
}
#define radix 10  //基数一般取10
void RadixSort(int a[],int n)  //基数排序
{
    int vmax = _max(a,n);  //最大值
    int exp = 1;  //比较位数，从个位开始比较
    int i;
    int *ans = (int*)malloc(sizeof(int)*n);
    while(vmax/exp>0)
    {
        int buckets[radix] = {0};  //桶要初始化为0
        for(i=0;i<n;++i)
            buckets[(a[i]/exp)%10]++;
        for(i=1;i<radix;++i)
            buckets[i] += buckets[i-1];
        for(i=n-1;i>=0;--i)
            ans[--buckets[(a[i]/exp)%10]] = a[i];
        for(i=0;i<n;i++)
            a[i] = ans[i];
        exp*=10;
    }
}

int main()
{
    clock_t start,end;
    int a[] = {1,5,2,7,10,13,9,15,6,23,4,21,17,12,0,8,19,5,6,7,9,10,14,10,23,12,25,34,17,10,31,21,23,28,20,27,16,17,1,14,18,25,19,
4,2,10,45,19,11,13,1,17,4,7,34,5,6,21,27,2,4,9,12,16,3,18,3,19,31,33,28,20,15,32,1,9,6,33,11,10,12,14,16,8,1,0,2,7,14,18,41,40,24,
20,41,43,25,6,8,14,9,2,14,16,3,31,37,29,38,26,10,24,33,42,44,15,16,5,3,7,13,25,7,35,6,26,7,2,11,6,33,6,2,1,9,2,34,5,32,1};
    int n = 140;
    start = clock();
    BubbleSort(a,n);
    PrintList(a,n);
    end = clock();
    double duration = (double)(end-start)/CLK_TCK;
    printf("BubbleSort time is %f\n",duration);
    return 0;
}

