#include <stdio.h>
#include <stdlib.h>

typedef struct nodetype
{
	index parent;
	int depth;
} universe;

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

	fseek(fp, SEEK_SET, 0);
	for (int i = 0; i < nodeNum; i++)
	{
		for (int j = 0; j < nodeNum; j++)
			fscanf_s(fp, "%d", &adj[i][j]);
	}
	for (int i = 0; i < nodeNum; i++)
	{
		for (int j = 0; j < nodeNum; j++)
			printf("%3d ", adj[i][j]);
		putchar('\n');
	}

	const int n = nodeNum;
	typedef int index;
	typedef index set_pointer;

	universe* U = (universe*)malloc(sizeof(universe) * n);
	if (U == NULL)
	{
		perror("Memory allocate error! - universe array");
		return -1;
	}

	//makeset
	for (int i = 0; i < nodeNum; i++)
	{
		U[i].parent = i;
		U[i].depth = 0;
	}

	free(U);
	free(adj[0]);
	free(adj);
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