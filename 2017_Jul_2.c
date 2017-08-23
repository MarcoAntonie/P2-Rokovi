#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_IN "poslovi.txt"
#define NAZIV 31
#define KATEGORIJA 21

#define ALLOC_CHECK(p) if(!(p)) printf("NO ALLOC"), exit(1)
#define FILE_CHECK(f) if(!(f)) printf("NO FILE"), exit(2)

typedef struct job
{
	char name[NAZIV], cat[KATEGORIJA];
	int w;
	struct job *next;
}Job;

Job *create_list(FILE *fin)
{
	Job *node = NULL, *prev = NULL, *pom;
	char name[NAZIV], cat[KATEGORIJA];
	int wage;

	while (fscanf(fin, "%30s:%20s:%d[\n]", &name, &cat, &wage) == 3)
	{
		Job *p = malloc(sizeof *p);
		ALLOC_CHECK(p);
		strcpy(p->name, name);
		strcpy(p->cat, cat);
		p->w = wage;

		if (node)
		{
			node = p;
		}
		else
		{
			pom = node;
			while (pom->w > p->w && pom)
			{
				prev = pom;
				pom = pom->next;
			}
			if (!prev)
				pom = p;
			else
				prev->next = p;
		}
	}
	return node;
}

void job_search(Job *node, char nc[KATEGORIJA], int w)
{
	Job *curr = NULL;

	printf("Poslovi koji odgovaraju su: \n");
	
	for (curr = node; curr; curr = curr->next)
	{
		if (!(strcmp(curr->cat, nc)) && curr->w == w)
		{
			printf("%s:%d\n", curr->name, w);
		}
	}
}

void clear_all(Job *node)
{
	while (node)
	{
		Job *p = node;
		node = p->next;
		free(p);
	}
}

int main(void)
{
	FILE *fin;
	Job *node;
	char nc[KATEGORIJA];
	int w;

	fin = fopen(FILE_IN, "r");
	FILE_CHECK(fin);
	create_list(fin);
	fclose(fin);

	printf("Zeljena kategorija? \n");
	gets(nc);
	printf("Minimalna plata? \n");
	scanf("%d\n", &w);

	job_search(node, nc, w);
	clear_all(node);

	return 0;
}