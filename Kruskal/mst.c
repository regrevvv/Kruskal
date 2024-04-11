#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	//input ���� ����
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

	//�ؽ�Ʈ���� ��� �� �о����
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

	//2���� �迭 ���� �Ҵ��� ������� ������ ����
	int** adj;
	if ((adj = (int**)malloc(sizeof(int*) * nodeNum)) == NULL) {
		perror("Memory allocate error!");
		return -1;
	}

	if ((adj[0] = (int*)malloc(sizeof(int) * nodeNum * nodeNum)) == NULL)
	{
		perror("Memory allocate error!");
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
			printf("%d ", adj[i][j]);
		putchar('\n');
	}


	free(adj[0]);
	free(adj);
	fclose(fp);
	return 0;
}