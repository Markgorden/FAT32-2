#include "structs.h"

extern struct BS_BPB BS_BPB;
extern struct FSI FSInfo;
extern FILE *f;

int info()
{
	long offset;
	offset=0;
	fseek(f, offset, SEEK_SET);
	fread(&BS_BPB, sizeof(struct BS_BPB), 1, f);
	offset=BS_BPB.FSInfo*BS_BPB.BytsPerSec;
	fseek(f, offset, SEEK_SET);
	fread(&FSInfo, sizeof(struct FSI), 1, f);
	
	printf("bytes per sector: %d\n", BS_BPB.BytsPerSec);
	printf("sectors per cluster: %d\n", BS_BPB.SecPerClus);
	printf("total sectors: %d\n", BS_BPB.TotSec32);
	printf("number of FATs: %d\n", BS_BPB.NumFATs);
	printf("sectors per FAT: %d\n", BS_BPB.FATSz32);
	printf("number of free sectors: %d\n", FSInfo.Free_Count);
	return 0;
}
