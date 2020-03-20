#### 图

##### 图的表示

- 邻接表表示

  空间需求$O(E+V)$,其中V表示顶点个数，E表示边数。适用于稀疏图。

- 邻接矩阵表示

  空间需求$\Theta(V^2)$，其中V表示顶点个数。适用于稠密图。

下面关于图的复杂度分析，如无说明均默认使用邻接表表示。

##### 拓扑排序

拓扑排序是对有向无圈图的顶点的一种排序，它使得如果存在一条从$v_i$到$v_j$的路径，那么排序中$v_j$出现在$v_i$的后面。图的拓扑排序，需要先求出各顶点的入度，顶点$v$的入度为边$(u,v)$的条数。图的拓扑排序使用队列实现，。时间复杂度$O(E+V)$。

##### 单源最短路径问题

1. 无权图

   无权图的单源最短路径问题，采用**广度优先搜索**方法，这是遍历图的一种重要的方法。时间复杂度$O(E+V)$。

2. 带权图

   带权图的单源最短路径问题采用Dijkstra算法，这是一个经典的贪婪算法。无法处理带有负边值的图。时间复杂度$O(ElogV)$。

##### 最小生成树问题

1. Prim算法

   不使用堆的时间复杂度$O(V^2)$,这对于稠密的图来说是最优的。使用堆的时间复杂度为$O(ElogV)$，对于稀疏图来说是一个好的时间界。

2. Kruskal算法

   贪婪算法，时间复杂度$O(ElogV)$。

##### 深度优先搜索

广度优先搜索外又一种重要的图的遍历方法，类似于树的先序遍历，采用递归实现，该方法保证每条边只访问一次，时间复杂度$O(E+V)$。

##### 多源最短路径问题

多源路径问题采用Floyd算法，该算法使用邻接矩阵存储图，时间复杂度$(V^3)$。

##### 代码

graph.h头文件，声明数据类型，定义初始化函数及测试函数

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <assert.h>
#define MAXSIZE 100
#ifndef _GRAPH_H
#define _GRAPH_H
void print(int* list,int size);      //一维数组输出例程
/**********************图的邻接表表示********************/
typedef char VertexType;  //定义节点类型
typedef struct Edge      //定义边
{     
    int index;            //index为边的末端，如果需要可以加上边的起始端
    int weight;          
    struct Edge* next;
}Edge;
struct Vertex        //定义节点
{        
    VertexType val;
    Edge* firstedge;
};
typedef struct mgraph   //定义图
{   
    int vexnum;          //节点数
    int edgenum;        //边数
    struct Vertex nodes[MAXSIZE];//节点表的最大节点数，还可以动态申请节点表，这样考虑有相应的Creat(malloc)与Dispose(free)例程
}Graph;
void Ini_Graph(Graph* G,int flag); //图的创建
void PrintG(Graph* G);             //输出邻接表图
int* Indegree(Graph* G);           //遍历整个图，返回节点入度
void PrintPath(int* Path,Graph* G,int start,int end); //显印出前置顶点表，采用递归
void Prim(Graph* G,int start);    //Prim算法
void Kruskal(Graph* G);          //Kruskal算法
void Dijkstra(Graph* G,int start); //Dijkstra算法
void bfs(Graph* G,int start);      //广度优先搜索
void dfs(Graph* G,int start,int* visited); //深度优先搜索
void TopSort(Graph* G);    //拓扑排序
/**********************图的邻接矩阵表示***************************/
typedef struct Mgraph  //图的邻接矩阵形式
{
    int vexnum;
    int edgenum;
    int** dis;
}Mgraph;
void printM(int** M,int Msize);  //输出邻接矩阵图
Mgraph* CreatG();               //邻接矩阵图的创建
void FreeG(Mgraph* G);          //邻接矩阵图的销毁
void IniG(Mgraph* G,int flag);  //邻接矩阵图的初始化
void Floyd(Mgraph* G);          //Floyd算法
/*************************************************/
typedef struct _edge{  //定义边，此处是为了Kruskal算法，需要边的起始点，如果在上面图中的边包含start信息，则不用这么麻烦
    int start;
    int end;
    int weight;
}_edge;

_edge** ReadG(Graph* G);                  //读取图，返回一个指针列表，列表中每一个指针指向一个_edge
_edge** sort(_edge** ptrlist,int size);   //对边按权排序，Kruskal算法的子例程，排序算法其实交换的是指针顺序
#endif

/***********************以下为图的邻接矩阵形式相应例程*******************/
void printM(int** M,int Msize)  
{
    for(int i=0;i<Msize;i++)
    {
        for(int j=0;j<Msize;j++)
            printf("%d ",M[i][j]);
        printf("\n");
    }
}
Mgraph* CreatG()     
{
    Mgraph* G =(Mgraph*)malloc(sizeof(Mgraph));
    printf("输入顶点数和边数:\n");
    scanf("%d%d",&G->vexnum,&G->edgenum);
    G->dis =(int**)malloc(G->vexnum*sizeof(int*));
    for(int i=0;i<G->vexnum;i++)
        G->dis[i] =(int*)malloc(G->vexnum*sizeof(int));
    return G;
}
void IniG(Mgraph* G,int flag)
{
    int i=0,j=0,weight=1;
    for(int k=0;k<G->vexnum;k++)
    {
        memset(G->dis[k],0x3f,G->vexnum*sizeof(int));
        G->dis[k][k]=0;
    }
    printf("输入边的两端和权:\n");
    for(int k=0;k<G->edgenum;k++)
    {
        scanf("%d%d%d",&i,&j,&weight);
        G->dis[i][j] = weight;
        if(!flag) //flag==0 为无向图 对于无向图，重复上面的操作i，j互换
            G->dis[j][i] = weight;
    }
}
void FreeG(Mgraph* G)  
{
    for(int i=0;i<G->vexnum;i++)
        free(G->dis[i]);
    free(G->dis);
    free(G);
}
/***********************以下为图的邻接表形式相应例程*****************/
void Ini_Graph(Graph* G,int flag)  //图的初始化
{
    Edge* p;
    int i=0,j=0,weight=1;
    printf("输入顶点数和边数:\n");
    scanf("%d%d",&G->vexnum,&G->edgenum);
    printf("输入顶点信息:\n");
    for(i=0;i<G->vexnum;i++)
    {
        getchar();
        scanf("%c",&G->nodes[i].val);  //必须有上面的getchar函数，直接scanf会把空格、换行符也当作一个字符
        G->nodes[i].firstedge=NULL;
    }
    printf("输入边的两端和权:\n");
    for(int k=0;k<G->edgenum;k++)
    {
        scanf("%d%d%d",&i,&j,&weight);
        p =(Edge*)malloc(sizeof(Edge));
        p->index = j;
        p->weight = weight;
        p->next = G->nodes[i].firstedge;  //头插法插入结点，对于一个新的节点，插入在头部
        G->nodes[i].firstedge = p;
        if(!flag) //flag==0 为无向图 对于无向图，重复上面的操作i，j互换
        {
            p = (Edge*)malloc(sizeof(Edge));
            p->index = i;
            p->weight = weight;
            p->next = G->nodes[j].firstedge;
            G->nodes[j].firstedge = p;
        }
        
    }
}

void PrintG(Graph* G)   //输出图
{
    Edge* p;
    for(int i=0;i<G->vexnum;i++)
    {
        p=G->nodes[i].firstedge;
        while(p)
        {
            printf("(%c,%c,%d)",G->nodes[i].val,G->nodes[p->index].val,p->weight);
            p = p->next;
        }
        printf("\n");
    }
}
void PrintPath(int* Path,Graph* G,int start,int end) //按前置顶点表输出路径 
{
    if(end==start)
        printf("%c ",G->nodes[start].val);
    else
    {
        PrintPath(Path,G,start,Path[end]);
        printf("to ");
        printf("%c ",G->nodes[end].val);
    }
   
}
int* Indegree(Graph* G)  //计算顶点入度
{
    int* indegree =(int*)calloc(G->vexnum,sizeof(int));
    for(int v=0;v<G->vexnum;v++)
    {
        for(Edge* E=G->nodes[v].firstedge;E!=NULL;E=E->next)
            indegree[E->index]++;
    }
    return indegree;
}
/**********************以下为内部函数，Kruskal算法子例程*************************/
_edge** ReadG(Graph* G)    //读取图中的边
{
    int counter = 0;
    _edge** ptrlist = (_edge**)calloc(G->edgenum,sizeof(_edge*));
    for(int i=0;i<G->vexnum;i++)
    {
        for(Edge* E=G->nodes[i].firstedge;E!=NULL;E=E->next)
        {
            if(i<E->index)  //这个判断，确保无向图中的边只被读取一次
            {
                _edge* ptr =(_edge*)calloc(1,sizeof(_edge));
                ptr->start = i;
                ptr->end = E->index;
                ptr->weight = E->weight;
                ptrlist[counter++] = ptr;
            }
        }
    }
    assert(counter == G->edgenum);
    return ptrlist;
}

_edge** sort(_edge** ptrlist,int size) //边按权排序 
{
    for(int i=0;i<size;i++)
    {
        int pos = i;
        int minweight = ptrlist[i]->weight;
        for(int j=i;j<size;j++)
            if(ptrlist[j]->weight < minweight)
            {
                minweight = ptrlist[j]->weight;
                pos = j;
            }
        _edge* tmp = ptrlist[pos];
        ptrlist[pos] = ptrlist[i];
        ptrlist[i] = tmp;
    }
    return ptrlist;
}

void print(int* list,int size) 
{
    for(int i=0;i<size;i++)
    {
        printf("%d ",list[i]);
    }
    printf("\n");
}
```

graph.c主文件，定义图的常见操作例程

```c
#include "graph.h"
#include "SetUnion.c"
#include "Queue.c"
void dfs(Graph* G,int start,int* visited)  //深度优先搜索，采用递归回溯思想
{
    visited[start]=1;
	printf("%c ",G->nodes[start].val);
	Edge* E;
	for(E=G->nodes[start].firstedge;E!=NULL;E=E->next)
	{
		if(!visited[E->index])
			dfs(G,E->index,visited);
	}
}

void bfs(Graph* G,int start)  //广度优先搜索，整体思路和Dijkstra算法一致，只是距离更新规则不同，此算法每个节点只会遍历一次
{
	Queue* Q =CreatQueue(G->vexnum);
	MakeEmpty(Q);
	int* Dis = (int*)malloc(G->vexnum*sizeof(int));
	memset(Dis,0xff,sizeof(int)*G->vexnum); //初始化距离为-1
	Enqueue(start,Q);
	Dis[start]=0;
	while(!IsEmpty(Q))
    {
        int v=Dequeue(Q);
        for(Edge* E=G->nodes[v].firstedge;E!=NULL;E = E->next)
        {
            if(Dis[E->index]==-1)
            {
                Dis[E->index] = Dis[v]+1;
                Enqueue(E->index,Q);
            }
        }
    }
    free(Q->list);
    DelQueue(Q);
    for(int i=0;i<G->vexnum;i++)
        printf("%d ",Dis[i]);
    free(Dis);
}

void Dijkstra(Graph* G,int start)    
{
	Queue* Q =CreatQueue(G->vexnum);
	MakeEmpty(Q);  //初始化队列
	int* Dis = (int*)malloc(G->vexnum*sizeof(int));
	memset(Dis,0x3f,sizeof(int)*G->vexnum);  //初始化距离为0x3f3f3f3f这是一个很大的数，可以代表正无穷
    int* Path = (int*)calloc(G->vexnum,sizeof(int));  //前置顶点表，用于输出最短路径
	Enqueue(start,Q);  //起始节点入队
	Dis[start]=0;
    while(!IsEmpty(Q))
    {
        int v = Dequeue(Q);
        for(Edge* E=G->nodes[v].firstedge;E!=NULL;E=E->next)
        {
            int newdis = Dis[v] + E->weight;  
            if(newdis < Dis[E->index])   //如果新的节点距离小于旧的距离，更新距离，节点入队
            {
                Dis[E->index] = newdis;
                Enqueue(E->index,Q);
                Path[E->index] = v;
            }
        }
    }
    print(Dis,G->vexnum);
    for(int k=0;k<G->vexnum;k++)
    {
        PrintPath(Path,G,start,k);
        printf("\n");
    }
    free(Q->list);
    DelQueue(Q);
    free(Path);
}

void Kruskal(Graph* G)  //无向图最小生成树，Kruskal算法
{
    _edge** edgelist = ReadG(G);
    edgelist = sort(edgelist,G->edgenum);  //按权值排序后的边
    int counter=0;
    int* set =(int*)malloc(G->vexnum*sizeof(int));      
    _edge* edges =(_edge*)calloc(G->vexnum-1,sizeof(int));
    IniSet(set,G->vexnum);  //初始化并查集
    int uroot,vroot;
    for(int i=0;counter<G->vexnum-1;i++)
    {
        _edge* e = edgelist[i];
        uroot = Find(set,e->start);     //查找边的起始节点所属集
        vroot = Find(set,e->end);       //查找边的终点所属集
        if(uroot != vroot)    //如果不在一个集中
        {
            SizeUnion(set,uroot,vroot);  //合并两个集
            edges[counter].start = e->start;
            edges[counter].end = e->end;
            edges[counter].weight = e->weight;
            counter++;
        }
    }
    for(int k=0;k<G->vexnum-1;k++)
        printf("%d %d %d\n",edges[k].start,edges[k].end,edges[k].weight);
    free(set);
    free(edgelist);
    free(edges);
}

void Prim(Graph* G,int start)  //无向图最小生成树
{ 
    int* Dis=(int*)malloc(G->vexnum*sizeof(int));
    memset(Dis,0x3f,G->vexnum*sizeof(int));  //此处的0x3f是整型，不能加引号
    Dis[start] = 0;                          //以上为初始化距离表
    int* Known =(int*)calloc(G->vexnum,sizeof(int));  //Known表，记录是否被遍历
    int* Path=(int*)calloc(G->vexnum,sizeof(int));  //前置顶点表
    int v = start;
    while(!Known[v])
    {
        Known[v] = 1;
        int Dismin = 0x3f3f3f3f;
        for(Edge* E = G->nodes[v].firstedge;E!=NULL;E=E->next)
        {
            if(!Known[E->index] && E->weight<Dis[E->index])   //如果节点未知，且边权值小于距离，刷新此节点距离
            {
                Dis[E->index] = E->weight;
                Path[E->index] = v;
            }
        }
        for(int i=0;i<G->vexnum;i++)
        {
            if(!Known[i] && Dis[i]<Dismin)  //在未知节点中找出距离最小节点，作为下一个节点
            {
                Dismin = Dis[i];
                v = i;
            }
        }
    }
    print(Dis,G->vexnum);
    for(int k=0;k<G->vexnum;k++)
    {
        PrintPath(Path,G,start,k);
        printf("\n");
    }   
    free(Dis);
    free(Known);
    free(Path);
}
void TopSort(Graph* G)   //图的拓扑排序
{
    int* indegree = Indegree(G); //先算出各顶点入度
	Queue* Q =CreatQueue(G->vexnum);
	MakeEmpty(Q);
    int* Topnum =(int*)calloc(G->vexnum,sizeof(int));
    int counter = 0;
    for(int i=0;i<G->vexnum;i++)
    {
        if(indegree[i] == 0)  //将入度为0的源点入队
            Enqueue(i,Q);
    }
    while(!IsEmpty(Q))
    {
        int v = Dequeue(Q);
        Topnum[counter++] = v;
        for(Edge* E = G->nodes[v].firstedge;E!=NULL;E=E->next)
        {
            if(--indegree[E->index]==0)  //将下一个入度为0的点入队
                Enqueue(E->index,Q);
        }
    }
    if(counter != G->vexnum)
        printf("Graph has a circle");
    else
    {
        for(int k=0;k<G->vexnum;k++)
            printf("%c ",G->nodes[Topnum[k]].val);
    }
    DelQueue(Q);
    free(Topnum);
    free(indegree);
}

void Floyd(Mgraph* G)   //Floyd算法解决多源最短路径问题
{
    for(int k=0;k<G->vexnum;k++)
        for(int i=0;i<G->vexnum;i++)
            for(int j=0;j<G->vexnum;j++)
                if(G->dis[i][j] > G->dis[i][k]+G->dis[k][j]) //如果中继后的距离小于不中继的距																//离，则刷新距离
                    G->dis[i][j] = G->dis[i][k]+G->dis[k][j];
}

int main()
{
  //  void Kruskal(Graph* G);
  //  void PrintG(Graph* G);
   // void Ini_Graph(Graph* G,int flag);
    //void dfs(Graph* G,int start,int* visited);
   // Mgraph* G = CreatG();
   // IniG(G,0);
   // printM(G->dis,G->vexnum);
   // Floyd(G);
   // printM(G->dis,G->vexnum);
   // FreeG(G);
      Graph G;
      Ini_Graph(&G,1);
      PrintG(&G);
     // int* visited = (int*)calloc(G.vexnum,sizeof(int));
  //    dfs(&G,0,visited);
     // free(visited);
      Dijkstra(&G,0);
  //  Kruskal(&G);
}
```

