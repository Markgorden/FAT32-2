#include <stdio.h>
#include "func.h"

extern int number_opened_file;
extern int number_read_file;
extern int number_write_file;
extern long opened_file[100];
extern long opened_read_file[100];
extern long opened_write_file[100];

void openfile(char *name, char *mode)
{
	int i, j;
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
			{
				opened_file[number_opened_file]=Offset;
				number_opened_file++;
				if(mode == "r"){
					opened_read_file[number_read_file] = Offset;
					number_read_file++;
					printf("File opened for reading.\n");
				{
				else if(mode == "w"){
					opened_write_file[number_write_file] = Offset;
					number_write_file++;
					printf("File opened for writing.\n");
				}
				else if(mode == "x"){
					opened_write_file[number_write_file] = Offset;
                                        number_write_file++;
					opened_read_file[number_read_file] = Offset;
                                        number_read_file++;
                                        printf("File opened for reading and writing.\n");
				}
					printf("Error! Invalid mode entered.\n");
			}

			else
				printf("Error! It's already opened.\n");
		}
		else
			printf("Error! It's not a file.\n");
	}
	else
		printf("Error! Entry not exists.\n");
}
