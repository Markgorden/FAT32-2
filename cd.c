#include "func.h"

extern char *working_directory;
extern uint32_t current_cluster;

int cd(char *name)
{
	int i,j;
	char file_name[12];
	struct DIR direntry;
	for (i=0; name[i]!='\0'; i++)
	{
		if (name[i]>= 'a' && name[i]<='z')
			name[i]-=32;
		file_name[i]=name[i];
	}
	for (; i<11; i++)
		file_name[i]=' ';
	file_name[i]='\0';
	
	if (strcmp(name, "/")==0)
	{
		working_directory[0]='/';
		working_directory[1]='\0';
		current_cluster=BS_BPB.RootClus;
			
		//parent_directory[0]='\0';
		//parent_cluster=-1;
	}
	else if (strcmp(name, ".")==0)
	{
		//?? If you do "cd ." in the root directory, what should happen?
	}
	else if (strcmp(name, "..")==0)
	{
		if (strcmp(working_directory, "/")==0)
		{
			printf("Error! It's in the root directory.\n");
			return -1;
		}
		else
		{
			for (i=0; working_directory[i]!='\0'; i++);	//go to the end
			for (i=i-1; working_directory[i]!='/'; i--);
			if (i!=0)
				working_directory[i]='\0';
			else
				working_directory[i+1]='\0';
			current_cluster=GetClusterOfPath(working_directory);
		}
	}
	else
	{
		direntry=FindDirentryOfFile(current_cluster, file_name);
		if (direntry.Name[0]==LAST_ENTRY)
			printf("Error! No such directory.\n");
		else
		{
			//printf("%d\n", current_cluster);
			if (direntry.Attr==0x10)
			{
				current_cluster=GetClusterOfDirentry(current_cluster, file_name);
				for (i=0; working_directory[i]!='\0'; i++);
				if (working_directory[i-1]!='/')
					working_directory[i++]='/';
				for (j=0; name[j]!='\0'; i++,j++)
					working_directory[i]=name[j];
				working_directory[i]='\0';
			}
			else
				printf("Error! It's not a directory.\n");
		}
	}
	return 0;
}
		
