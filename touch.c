#include <stdio.h>
#include "func.h"

extern char *working_directory;
extern uint32_t current_cluster;
extern FILE *f;

int touch (char *name)
{
	int i, j;
	long Offset;
	uint32_t new_cluster;
	char file_name[12];
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
	
	//printf("1%s1\n", file_name);
	//sleep(5);
	
	direntry=FindDirentryOfFile(current_cluster, file_name);
	if (direntry.Name[0]==LAST_ENTRY)
	{
		Offset=FindEmptyentryOfCluster(current_cluster);
		//fseek(f, Offset, SEEK_SET);
		//fread(empty_entry, sizeof(struct DIR), 1, f);
		//fprintf(f, "a");
		//sleep(10);
		for (i=0; i<11; i++)
			empty_entry.Name[i]=file_name[i];
		empty_entry.Attr=0x20;
		empty_entry.NTRes=0x00;
		empty_entry.FileSize=0x00000000;
		//printf("Here!\n");
		//printf("0x%lx\n", Offset);
		
		//sleep(10);
		
		fseek(f, BS_BPB.FSInfo*BS_BPB.BytsPerSec, SEEK_SET);
		fread(&FSInfo, sizeof(struct FSI), 1, f);
		
		if (FSInfo.Nxt_free==0xFFFFFFFF)
			new_cluster=0x00000002;
		else
			new_cluster=FSInfo.Nxt_free+1;
		//printf("0x%x\n", new_cluster);
		for ( ; ; new_cluster++)
		{
			if (FAT(new_cluster)==0x00000000)
			{
				//printf("0x%x\n", new_cluster);
				empty_entry.FstClusHI=(new_cluster>>16);
				empty_entry.FstClusLO=(new_cluster & 0x0000FFFF);	//suspecious
				ChangeValueOfCluster(EOC, new_cluster);
				FSInfo.Nxt_free=new_cluster;
				fseek(f, BS_BPB.FSInfo*BS_BPB.BytsPerSec, SEEK_SET);
				fwrite(&FSInfo, sizeof(struct FSI), 1, f);
				fflush(f);
				break;
			}
			if (new_cluster==0xFFFFFFFF)
				new_cluster=0x00000001;
		}
		//printf("0x%lx\n", Offset);
		fseek(f, Offset, SEEK_SET);
		fwrite(&empty_entry, sizeof(struct DIR), 1, f);
		fflush(f);
	}
	else
		printf("Error! Entry already exists.\n");
}
