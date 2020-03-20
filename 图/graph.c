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
                if(G->dis[i][j] > G->dis[i][k]+G->dis[k][j])     //如果中继后的距离小于不中继的距离，则刷新距离
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
