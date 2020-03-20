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
/*******************************以下为内部例程，Kruskal算法子例程******************************/
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

