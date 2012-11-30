#include <stdio.h>
#include "func.h"

extern char *working_directory;
extern uint32_t current_cluster;
extern FILE *f;

int rm (char *name)
{
	int i, j;
	long Offset;
	uint32_t next_cluster;
	char file_name[12];
	char empty[32];
	struct DIR empty_entry, direntry;
	struct FSI FSInfo;
	
	for (i=0; name[i]!='\0'; i++)
	{
		if (name[i]>= 'a' && name[i]<='z')
			name[i]-=32;
	}
	for (i=0; i<8; i++)
	{
		if (name[i]!='\0' && name[i]!='.')
			file_name[i]=name[i];
		else
			break;
	}
	for (j=i; j<8; j++)
		file_name[j]=' ';
	if (name[i]=='.')
	{
		i++;
		j=8;
		for (j=8; j<11; i++, j++)
		{
			if (name[i]!='\0')
				file_name[j]=name[i];
			else
				break;
		}
		for ( ; j<12; j++)
			file_name[j]=' ';
	}
	else
	{
		for ( ; i<11; i++)
			file_name[i]=' ';
	}
	file_name[11]='\0';
	
	for (i=0; i<32; i++)
		empty[i]='\0';
	
	direntry=FindDirentryOfFile(current_cluster, file_name);
	if (direntry.Name[0]!=LAST_ENTRY)				//haven't check for whether it is opened, will be added
	{
		if (direntry.Attr==0x20)
		{
			Offset=GetOffsetOfEntry(current_cluster, file_name);
			next_cluster=(direntry.FstClusHI << 16 | direntry.FstClusLO);
			EmptyValueOfCluster(next_cluster);
			fseek(f, Offset, SEEK_SET);
			fwrite(&empty, 32, 1, f);
		}
		else
			printf("Error! It's not a file.\n");
	}
	else
		printf("Error! Entry not exists.\n");
}
