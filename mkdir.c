#include <stdio.h>
#include "func.h"

extern char *working_directory;
extern uint32_t current_cluster;
extern FILE *f;

int mkdir (char *name)
{
	int i;
	long Offset;
	uint32_t new_cluster;
	char entry_name[12];
	struct DIR empty_entry, direntry;
	struct FSI FSInfo;
	
	for (i=0; name[i]!='\0'; i++)
	{
		if (name[i]>= 'a' && name[i]<='z')
			name[i]-=32;
	}
	for (i=0; i<8; i++)
	{
		if (name[i]!='\0')
			entry_name[i]=name[i];
		else
			break;
	}
	for (; i<11; i++)
		entry_name[i]=' ';
	entry_name[11]='\0';
	
	direntry=FindDirentryOfFile(current_cluster, entry_name);
	if (direntry.Name[0]==LAST_ENTRY)
	{
		Offset=FindEmptyentryOfCluster(current_cluster);
		for (i=0; i<11; i++)
			empty_entry.Name[i]=entry_name[i];
		empty_entry.Attr=0x10;
		empty_entry.NTRes=0x00;
		empty_entry.FileSize=0x00000000;
		
		fseek(f, BS_BPB.FSInfo*BS_BPB.BytsPerSec, SEEK_SET);
		fread(&FSInfo, sizeof(struct FSI), 1, f);
		
		if (FSInfo.Nxt_free==0xFFFFFFFF)
			new_cluster=0x00000002;
		else
			new_cluster=FSInfo.Nxt_free+1;
		
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
		
	Offset=FindEmptyentryOfCluster(new_cluster);
	empty_entry.Name[0]='.';
	for (i=1; i<12; i++)
		empty_entry.Name[i]=' ';
	empty_entry.Attr=0x10;
	empty_entry.NTRes=0x00;
	empty_entry.FileSize=0x00000000;
	empty_entry.FstClusHI=(new_cluster>>16);
	empty_entry.FstClusLO=(new_cluster & 0x0000FFFF);
	fseek(f, Offset, SEEK_SET);
	fwrite(&empty_entry, sizeof(struct DIR), 1, f);
	
	Offset=FindEmptyentryOfCluster(new_cluster);
	empty_entry.Name[0]='.';
	empty_entry.Name[1]='.';
	for (i=2; i<12; i++)
		empty_entry.Name[i]=' ';
	empty_entry.Attr=0x10;
	empty_entry.NTRes=0x00;
	empty_entry.FileSize=0x00000000;
	empty_entry.FstClusHI=0x0000;
	empty_entry.FstClusLO=0x0000;
	fseek(f, Offset, SEEK_SET);
	fwrite(&empty_entry, sizeof(struct DIR), 1, f);
}
