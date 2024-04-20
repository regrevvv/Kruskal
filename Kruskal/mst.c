#include <stdio.h>
#include <stdlib.h>

typedef int index;
typedef index set_pointer;
typedef struct nodetype
{
	index parent;
	int depth;
} universe;

typedef struct edge
{
	int weight;
	index p;
	index q;
}Edge;

int cmpfunc(const void*, const void*);
set_pointer	find(universe*, int);
void merge(universe*, set_pointer, set_pointer);

int main(int argc, char** argv)
{
	//input 파일 열기
	FILE* fp;
	if (argc != 2)
	{
		printf("Needs one parameter - file name that contains adjacency matrix.");
		return -1;
	}

	if ((fopen_s(&fp, argv[1], "r")) != 0)
	{
		perror("Failed to open file.");
		return -1;
	}

	//텍스트에서 노드 수 읽어오기
	int loop = 1, nodeNum = 0;
	while (loop)
	{
		switch (fgetc(fp))
		{
		case ' ':
			nodeNum++;
			break;
		case '\n':
			nodeNum++;
			loop = 0;
			break;
		}
	}

	//2차원 배열 동적 할당후 인접행렬 데이터 삽입
	int** adj;
	if ((adj = (int**)malloc(sizeof(int*) * nodeNum)) == NULL) {
		perror("Memory allocate error! - adjacent matrix");
		return -1;
	}

	if ((adj[0] = (int*)malloc(sizeof(int) * nodeNum * nodeNum)) == NULL)
	{
		perror("Memory allocate error! - adjacent matrix");
		return -1;
	}
	for (int i = 1; i < nodeNum; i++)
		adj[i] = adj[i - 1] + 4;


	int** mst;//결과를 저장할 배열
	if ((mst = (int**)malloc(sizeof(int*) * nodeNum)) == NULL) {
		perror("Memory allocate error! - adjacent matrix");
		return -1;
	}

	if ((mst[0] = (int*)malloc(sizeof(int) * nodeNum * nodeNum)) == NULL)
	{
		perror("Memory allocate error! - adjacent matrix");
		return -1;
	}
	for (int i = 1; i < nodeNum; i++)
		mst[i] = mst[i - 1] + 4;

	fseek(fp, SEEK_SET, 0);
	for (int i = 0; i < nodeNum; i++)
	{
		for (int j = 0; j < nodeNum; j++)
			fscanf_s(fp, "%d", &adj[i][j]);
	}

	//출력하기

	putchar(' ');
	for (int i = 0; i < nodeNum; i++)
	{
		printf("%4c", i + 65);
	}
	for (int i = 0; i < nodeNum; i++)
	{
		putchar('\n');
		printf("%c", i + 65);
		for (int j = 0; j < nodeNum; j++)
			printf("%4d", adj[i][j]);
	}

	int edgeNum = 0;
	for (int i = 0; i < nodeNum; i++)
		for (int j = i + 1; j < nodeNum; j++)
			if (adj[i][j] > 0)
				edgeNum++;
	Edge* sortedEdge = (Edge*)malloc(sizeof(Edge) * edgeNum);

	for (int i = 0, k = 0; i < nodeNum; i++)
		for (int j = i + 1; j < nodeNum; j++)	
			if (adj[i][j] > 0)
			{
				sortedEdge[k].weight = adj[i][j];
				sortedEdge[k].p = i;
				sortedEdge[k++].q = j;
			}

	qsort((Edge*)sortedEdge, edgeNum, sizeof(Edge), cmpfunc);

	const int n = nodeNum;

	universe* U = (universe*)malloc(sizeof(universe) * n);
	if (U == NULL)
	{
		perror("Memory allocate error! - universe array");
		return -1;
	}


	//makeset + initial
	for (int i = 0; i < nodeNum; i++)
	{
		U[i].parent = i;
		U[i].depth = 0;
	}



	free(U);
	free(sortedEdge);
	free(adj[0]);
	free(adj);
	free(mst[0]);
	free(mst);
	fclose(fp);
	return 0;
}

set_pointer	find(universe* U, int i)
{
	int j = i;
	while (U[j].parent != j)
		j = U[j].parent;
	return j;
}

void merge(universe* U, set_pointer p, set_pointer q) {
	if (U[p].depth == U[q].depth)
	{
		U[p].depth += 1;
		U[q].parent = p;
	}
	else if (U[p].depth < U[q].depth)
		U[q].parent = q;
	else
		U[p].parent = p;
}

int equal(set_pointer p, set_pointer q)
{
	if (p == q)
		return 1;
	else
		return 0;
}

int cmpfunc(const void* a, const void* b)
{
	const Edge* tmpa = a;
	const Edge* tmpb = b;

	return tmpa->weight - tmpb->weight;
}