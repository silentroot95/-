##### 排序

这里的排序指内排序。

插入排序

- 插入排序
- 希尔排序

选择排序

- 选择排序
- 堆排序

交换排序

- 冒泡排序
- 快速排序

归并排序

桶排序

基数排序

##### 插入排序

插入排序的思想就像整理扑克牌，对于新来的一个值，将它插入两个值之间。平均时间复杂度$O(n^2)$,

空间复杂度O(1)，稳定。插入排序，对较少的数据效率很高。

```c
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
```

##### 希尔排序

希尔排序与插入排序的思想一样，不过有一个间距序列，此序列最终要取到1，时间复杂度$O(nlogn)$,

空间复杂度O(1),不稳定。

```c
void ShellSort(int a[],int n)
{
    int step,p,i,j,tmp;
    for(step=n/2;step>0;step/=2)  //step为每次插入排序的间距，最终要取到1，
 								 //即最后一次执行的是插入排序，不过此时数组大部分已是有序的
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
```

##### 选择排序

选择排序，是先选出最小值（升序排序），放在第一位，再选出次小值，依次类推。时间复杂度$O(n^2)$,

空间复杂度O(1)，不稳定。

```c
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
```

##### 堆排序

堆排序（升序排序）,用原数组建立一个大顶堆，然后依次弹出堆顶元素（最大元），将最大元与最后元素替换，然后维持堆序性。时间复杂度$O(nlogn)$，空间复杂度O(1)，不稳定。

```c
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
```

##### 冒泡排序

冒泡排序，依次比较相邻的元素，如果逆序就交换他们，每一轮操作都会使得最大的元素沉到最下面，小的元素浮到上面。时间复杂度$O(n^2)$,空间复杂度O(1),稳定。

```c
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
```

##### 快速排序

快速排序也是基于交换的思想，快速排序，采用分治策略递归实现。每次选择一个枢纽元，把元素分为大于枢纽元与小于枢纽元两部分。时间复杂度$O(nlogn)$,由于使用递归空间复杂度$O(logn)$,最差情况下可能为O(n),不稳定。

```c
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
```

##### 归并排序

归并排序，采用典型的递归分治思想，把一个序列分为两部分，两部分分别排序后，再合并为一个序列，时间复杂度$O(nlogn)$，空间复杂度O(n)，稳定。

```C
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
void MergeSort(int a[],int n) //归并排序主例程
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
```

##### 桶排序

桶排序，采用一遍扫描序列，把值放到相应的桶中，然后逆序扫描序列把值放到相应的位置。桶排序，适用于元素值在一定范围内的序列如[0,k]，这个k不能太大，否则需要很多的桶。时间复杂度$O(n+k)$,空间复杂度O(n+k)，稳定。

```c
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
```

##### 基数排序

基数排序，实际上是多趟桶排序，时间复杂度$O(r(n+k))$,其中r为最大值位数，n为数组长度，k为桶数量，空间复杂度$O(n+k)$,稳定。

```c
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
```

