#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

char* differfilename(char*);
char* reverse(char*);

char* differfilename(char *path)
{
	int i;
	int j=0;
	int len;
	char *filename;

	filename=(char*)malloc(sizeof(char)*20);

	if(!filename)
	{
		perror("malloc");
		goto OUT;
	}
	
	len=strlen(path);

	i=len-1;

	while(1)
	{
		if(*(path+i)=='/')
		{
			break;
		}

		*(filename+j) = *(path+i);

		i--;
		j++;		
	}

	*(filename+j)='\0';

	filename=reverse(filename);

	return filename;
OUT:
	return 0;
}

char* reverse(char *filename)
{
	int i;
	int j;
	int len;
	char *rev;

	len=strlen(filename);
	
	j=len-1;

	rev=(char*)malloc(sizeof(char)*(len+1));

	if(!rev)
	{
		perror("malloc");
		goto OUT;
	}

	for(i=0;i<len;i++)
	{
		*(rev+i)=*(filename+j);

		j--;
	}

	return rev;
OUT:
	return 0;
}
