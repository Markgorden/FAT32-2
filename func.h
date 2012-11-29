#include "structs.h"

extern struct BS_BPB BS_BPB;
extern FILE *f;

#define EMPTY_ENTRY 0xE5
#define LAST_ENTRY 0x00
#define ATTR_LONG_NAME 0x0F

#ifndef GOOS
#define GOOS 1
long GetOffsetOfSector(long sector)
{
	return (sector*BS_BPB.BytsPerSec);
}
#endif

#ifndef FSOC
#define FSOC 1
long FirstSectorOfCluster(uint32_t cluster)
{
	return ((cluster-2)*BS_BPB.SecPerClus + BS_BPB.RsvdSecCnt + BS_BPB.FATSz32*2);
}
#endif

#ifndef FA
#define FA 1
uint32_t FAT(uint32_t cluster)
{
	uint32_t next_cluster;
	long offset;
	offset=BS_BPB.RsvdSecCnt*BS_BPB.BytsPerSec + cluster*4;
	fseek(f, offset, SEEK_SET);
	fread(&next_cluster, sizeof(uint32_t), 1, f);
	return next_cluster;
}
#endif

#ifndef E
#define E 1
int equal(uint8_t name1[], uint8_t name2[])
{
	int i;
	for (i=0; i<11; i++)
	{
		if (name1[i]!=name2[i])
			return 0;
	}
	return 1;
}
#endif

#ifndef FDOF
#define FDOF 1
struct DIR FindDirentryOfFile(uint32_t cluster, char *name)
{
	int i;
	long Offset;
	char file_name[12];
	struct DIR direntry;
	while (1)
	{
		Offset=GetOffsetOfSector(FirstSectorOfCluster(cluster));
		fseek(f, Offset, SEEK_SET);
		while (Offset < GetOffsetOfSector(FirstSectorOfCluster(cluster))+BS_BPB.BytsPerSec*BS_BPB.SecPerClus)
		{
			fread(&direntry, sizeof(struct DIR), 1, f);
			Offset+=32;
			if (direntry.Name[0] == EMPTY_ENTRY)
				continue;
			else if (direntry.Name[0] == LAST_ENTRY)
				break;
			else if (direntry.Name[0] == 0x05)
				direntry.Name[0] = 0xE5;
			if (direntry.Attr!=ATTR_LONG_NAME)
			{
				//printf("%s    %s\n", direntry.Name, name);
				for (i=0; i<11; i++)
					file_name[i]=direntry.Name[i];
				file_name[11]='\0';
				if (strcmp(file_name, name)==0)
					return direntry;
			}
		}
		cluster=FAT(cluster);
		if (cluster>=EOC)
			break;
	}
	direntry.Name[0]=LAST_ENTRY;
	return direntry;
}
#endif

#ifndef GCOD
#define GCOD 1
uint32_t GetClusterOfDirentry(uint32_t cluster, char *name)
{
	int i;
	char file_name[12];
	long Offset;
	struct DIR direntry;
	while (1)
	{
		Offset=GetOffsetOfSector(FirstSectorOfCluster(cluster));
		//printf("%ld\n", Offset);
		fseek(f, Offset, SEEK_SET);
		while (Offset < GetOffsetOfSector(FirstSectorOfCluster(cluster))+BS_BPB.BytsPerSec*BS_BPB.SecPerClus)
		{
			//read_32_bytes(direntry)
			fread(&direntry, sizeof(struct DIR), 1, f);
			Offset+=32;
			if (direntry.Name[0] == EMPTY_ENTRY)
				continue;
			else if (direntry.Name[0] == LAST_ENTRY)
				break;
			else if (direntry.Name[0] == 0x05)
				direntry.Name[0] = 0xE5;
			//printf("%s\n",direntry.Name);
			if (direntry.Attr!=ATTR_LONG_NAME)
			{
				//printf("%s    %s\n", direntry.Name, name);
				for (i=0; i<11; i++)
					file_name[i]=direntry.Name[i];
				file_name[11]='\0';
				if (strcmp(file_name, name)==0)
					return (direntry.FstClusHI << 16 | direntry.FstClusLO);
			}
		}
		cluster = FAT(cluster);
		if (cluster >= EOC)
			break;
	}
}
#endif

#ifndef GCOP
#define GCOP 1	
long GetClusterOfPath(char *str)
{
	int i, j;
	long cluster;
	uint8_t name[11];
	cluster=BS_BPB.RootClus;
	while(1)
	{
		j=0;
		for (i=1; ; i++)
		{
			if (str[i]=='/' || str[i]=='\0')
			{
				name[j]='\0';
				break;
			}
			else
				name[j]=str[i];
		}
		if (strcmp(name, "")!=0)
			cluster=GetClusterOfDirentry(cluster, name);
		else
			break;
	}
	return cluster;
}
#endif
