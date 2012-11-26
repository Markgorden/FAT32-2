#include "structs.h"

extern struct BS_BPB BS_BPB;

int ls()
{
	while (1)
	{
		Offset=(32+1009*2)*512;
		while (Offset < (32+1009*2)*512+512*1)
		{
			read_32_bytes(direntry)
			if (direntry.name[0] == LAST_ENTRY)
				break;
			else if (direntry.name[0] == EMPTY_ENTRY)
				continue
			name = append(direntry.name, '\0')
			print(name)
			C = FAT(C)
			if (C >= EOC)
				break;
		}
	return 0;
}
