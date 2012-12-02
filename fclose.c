#include <stdio.h>
#include "func.h"

extern int number_opened_file;
extern long opened_file[100];

void closefile(char *name)
{
	int i, j, thismode = 0;
	long Offset;
	char file_name[12];
	struct DIR direntry;
	
	
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
	
	direntry=FindDirentryOfFile(current_cluster, file_name);
	if (direntry.Name[0]!=LAST_ENTRY)
	{
		if (direntry.Attr==0x20)
		{
			Offset=GetOffsetOfEntry(current_cluster, file_name);
			if (NotOpened(Offset))
				printf("Error! It's not opened.\n");
			else
			{
				for (i=0; i<number_opened_file; i++){
					if (opened_file[i]==Offset)
						break;
				}	
				for (j=i; j<number_opened_file-1; j++)
					opened_file[j]=opened_file[j+1];
				opened_file[j]=0;
				number_opened_file--;

				for(i=0; i < number_read_file; i++){
					if (opened_read_file[i] == Offset){
						break; 
					}
				}
				for (j=i; j<number_read_file-1; j++)
                                        opened_read_file[j]=opened_read_file[j+1];
                                opened_read_file[j]=0;
                                number_read_file--;

				for(i=0; i < number_write_file; i++){
                                        if (opened_write_file[i] == Offset){
                                                break;
                                        }
                                }
                                for (j=i; j<number_write_file-1; j++)
                                        opened_write_file[j]=opened_write_file[j+1];
                                opened_write_file[j]=;
                                number_write_file--;

			}
		}
		else
			printf("Error! It's not a file.\n");
	}
	else
		printf("Error! Entry not exists.\n");
}
