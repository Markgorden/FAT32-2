#include "structs.h"

#define EMPTY_ENTRY 0xE5
#define LAST_ENTRY 0x00
#define ATTR_LONG_NAME 0x0F

extern FILE *f;
extern struct DIR direntry;
extern struct BS_BPB BS_BPB;

int ls()
{
	int i;
	long offset;
	char name[12];
	
	offset=(32+1009*2)*512;
	
	while (1)
	{		
		if (offset < (32+1009*2)*512+512*1)
		{
			//read_32_bytes(direntry)
			
			fseek(f, offset, SEEK_SET);
			fread(&direntry, sizeof(struct DIR), 1, f);
			
			if (direntry.Name[0] == LAST_ENTRY)
				break;
			else if (direntry.Name[0] == EMPTY_ENTRY)
			{
				offset+=32;
				continue;
			}
			//name = append(direntry.name, '\0')
			if (direntry.Attr!=ATTR_LONG_NAME)
			{
				for (i=0; i<11; i++)
				{
					name[i]=direntry.Name[i];
				}
				name[11]='\0';
				//print(name)
				printf("%s\n",name);
				
				//printf("%ld\n", offset);
			}
			offset+=32;
		}
	}
	return 0;
}
