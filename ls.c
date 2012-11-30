#include "structs.h"
#include "func.h"

#define EMPTY_ENTRY 0xE5
#define LAST_ENTRY 0x00
#define ATTR_LONG_NAME 0x0F

extern uint32_t current_cluster;
extern FILE *f;
//extern struct DIR direntry;
extern struct BS_BPB BS_BPB;
extern char *work_directory;

int ls()
{
	int i;
	long Offset;
	uint32_t C;
	char name[9], ext[4];
	struct DIR direntry;
	C=current_cluster;
	
	while (1)
	{
		Offset = GetOffsetOfSector(FirstSectorOfCluster(C));
		fseek(f, Offset, SEEK_SET);
	
		while (Offset < GetOffsetOfSector(FirstSectorOfCluster(C))+BS_BPB.BytsPerSec*BS_BPB.SecPerClus)
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
			//name = append(direntry.name, '\0')
			if (direntry.Attr!=ATTR_LONG_NAME)
			{
				for (i=0; i<8; i++)
				{
					if (direntry.Name[i]!=' ')
						name[i]=direntry.Name[i];
					else
					{
						name[i]='\0';
						break;
					}
				}
				name[8]='\0';
				for (i=0; i<3; i++)
				{
					if (direntry.Name[i+8]!=' ')
						ext[i]=direntry.Name[i+8];
					else
					{
						ext[i]='\0';
						break;
					}
				}
				ext[3]='\0';
				//print(name)
				//printf("Offset=%ld\n", Offset);
				if (strcmp(name, ".")!=0 && strcmp(name, "..")!=0)
				{
					if (ext[0]!='\0')
						printf("%s.%s\n", name,ext);
					else
						printf("%s\n", name);
				}
				//printf("%ld\n", offset);
			}
		}
		C = FAT(C);
		if (C >= EOC)
			break;
	}
	return 0;
}
