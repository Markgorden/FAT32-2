#include <stdio.h>
#include <stdlib.h>
#include "fat.h"
#include "structs.h"

char *image_name;
char *working_directory;
uint32_t current_cluster;
char *parent_directory;
uint32_t parent_cluster;

FILE *f;
struct BS_BPB BS_BPB;
//struct FSI FSInfo;
//struct DIR direntry;

int main(int argc, char *argv[])
{
	int i;
	char name[13];
	char operation[6];
	working_directory=(char*)malloc(200*sizeof(char));
	parent_directory=(char*)malloc(200*sizeof(char));
	
	if (argc == 2)
	{
		if (f = fopen(argv[1], "rb+"))
		{
			fread(&BS_BPB, sizeof(struct BS_BPB), 1, f);
			image_name=argv[1];
			
			working_directory[0]='/';
			working_directory[1]='\0';
			current_cluster=BS_BPB.RootClus;
			
			parent_directory[0]='\0';
			parent_cluster=-1;
			
			while(1)
			{
				printf("%s:%s>", image_name, working_directory);
				scanf("%s",operation);
				if (strcmp(operation, "info")==0)
					info();
				else if (strcmp(operation, "ls")==0)
					ls();
				else if (strcmp(operation, "cd")==0)
				{
					scanf("%s", name);
					getchar();
					cd(name);
				}
				else if (strcmp(operation, "touch")==0)
				{
					scanf("%s", name);
					getchar();
					touch(name);
				}
				/*else if (strcmp(operation, "size")==0)
					size();
				else if (strcmp(operation, "fopen")==0)
					fopen();
				else if (strcmp(operation, "fclose")==0)
					fclose();
				else if (strcmp(operation, "fread")==0)
					fread();
				else if (strcmp(operation, "fwrite")==0)
					fwrite();*/
				else if (strcmp(operation, "rm")==0)
				{
					scanf("%s", name);
					getchar();
					rm(name);
				}
				else if (strcmp(operation, "mkdir")==0)
				{
					scanf("%s", name);
					getchar();
					mkdir(name);
				}
				/*else if (strcmp(operation, "rmdir")==0)
					rmdir();*/
				else if (strcmp(operation, "exit")==0)
				{
					fclose(f);
					break;
				}
				else
					printf("Argument not correct\n");
				
			}
			return 0;
		}
		else
		{
			printf("Image not found!\n");
			return -1;
		}
	}
	else
	{
		printf("Arguments not correct!\n");
		return -1;
	}
}
