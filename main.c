#include <stdio.h>
#include <stdlib.h>
#include "fat.h"
#include "structs.h"

char *image_name;
char *working_directory;
uint32_t current_cluster;
char *parent_directory;
uint32_t parent_cluster;
int number_opened_file;
long opened_file[100];
int number_read_opened;
int number_write_opened;
long opened_read_file[100];
long opened_write_file[100];

FILE *f;
struct BS_BPB BS_BPB;
//struct FSI FSInfo;
//struct DIR direntry;

int main(int argc, char *argv[])
{
	int i;
	char mode[1];
	char name[13];
	char operation[6];
	working_directory=(char*)malloc(200*sizeof(char));
	parent_directory=(char*)malloc(200*sizeof(char));
	
	number_opened_file=0;
	number_read_file= 0;
	number_write_file= 0;
	for (i=0; i<100; i++){
		opened_file[i]=0;
		opened_read_file[i] = 0;
		opened_write_file[i] = 0;
	}
	
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
					size();*/
				else if (strcmp(operation, "fopen")==0)
				{
					scanf("%s", name);
					scanf("%s", mode);
					getchar();
					openfile(name, mode);
				}
				else if (strcmp(operation, "fclose")==0)
				{
					scanf("%s", name);
					getchar();
					closefile(name);
				}
				/*else if (strcmp(operation, "fread")==0)
					readfile();
				else if (strcmp(operation, "fwrite")==0)
					writefile();*/
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
				else if (strcmp(operation, "rmdir")==0)
				{
					scanf("%s", name);
					getchar();
					rmdir(name);
				}
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
