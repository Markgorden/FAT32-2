#include "structs.h"

extern struct BS_BPB BS_BPB;

int info()
{
	printf("bytes per sector: %d\n", BS_BPB.BytsPerSec);
	printf("sectors per cluster: %d\n", BS_BPB.SecPerClus);
	printf("total sectors: %d\n", BS_BPB.TotSec32);
	printf("number of FATs: %d\n", BS_BPB.NumFATs);
	printf("sectors per FAT: %d\n", BS_BPB.FATSz32);
	printf("number of free sectors: %d\n", BS_BPB.TotSec32-BS_BPB.NumFATs*BS_BPB.FATSz32-BS_BPB.RsvdSecCnt);
	return 0;
}
