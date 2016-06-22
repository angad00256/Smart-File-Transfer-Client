#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

char* differfilename(char*);
char* reverse(char*);

int main()
{
	int ret;
	int stop;
	char choice;
	char *filename;
	char *path;
	int pfd;
	int sockid;
	int connectid;
	struct sockaddr_in caddr;
	int i=0;
	int fd;
	int count;
	char ch;
	char security;
	char *key;
	int uack;
	int dack;
	int dwnsec;
	int dfd;
	int kack;

	sockid=socket(AF_INET,SOCK_STREAM,0);
			
	caddr.sin_family=AF_INET;
	caddr.sin_port=htons(4005);
	caddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	connectid=connect(sockid,(struct sockaddr *)&caddr,sizeof(caddr));

	printf("\n___FILE TRANSFER MENU___\n\n");

	printf("1. Upload a file \n");
	printf("2. Download a file \n");
	printf("0. Exit \n");

	printf("\nEnter your choice : "); 

	scanf("%c",&choice);

	switch(choice)
	{
		case '0': write(sockid,&choice,1);
			  break;

		case '1': write(sockid,&choice,1);

			path=(char*)malloc(sizeof(char)*50);

			filename=(char*)malloc(sizeof(char)*20);

			ret=fork();
	
			if(ret==0)
			{
      				execl("fileselect","fileselect",NULL);
			}
			
			wait(&stop);

			pfd=open("uploadfilepath",O_RDONLY);

			i=0;

			while(1)
			{
				read(pfd,(path+i),1);

				if(*(path+i)==10)
				{
					*(path+i)='\0';
					break;
				}

				i++;
			}
		
			filename = differfilename(path);
		
			i=0;

			while(1)
			{
				write(sockid,(filename+i),1);
				
				if(*(filename+i)=='\0')
				{
					break;
				}
				
				i++;
			}

			read(sockid,&uack,sizeof(int));

			if(uack==-1)
			{
				printf("\nSorry sir, this file is already uploaded on the server \n");
				
				break;				
			}

			fd=open(path,O_RDONLY);

			i=0;

			while(1)
			{
				count=read(fd,&ch,1);
			
				write(sockid,&count,sizeof(int));
	
				if(count==0)
				{
					break;
				}
		
				write(sockid,&ch,1);
			}
		
			printf("\n");

			while(1)
			{		
				printf("\rDo you want to add security to your file (y/n) : ");
				scanf("%c",&security);
	
				if((security=='y')||(security=='n'))
				{
					write(sockid,&security,1);
					break;
				}	
			}

			if(security=='y')
			{
				key=(char*)malloc(sizeof(char)*20);

				printf("\nEnter the password to be set for this file : ");

				scanf("%s",key);
				
				i=0;

				while(1)
				{
					write(sockid,(key+i),1);
				
					if(*(key+i)=='\0')
					{
						break;
					}
				
					i++;
				}
			}

			read(sockid,&uack,sizeof(int));

			if(uack==1)
			{
				printf("\nFile uploaded successfully with added security \n\n");
			}

			else if(uack==0)
			{
				printf("\nFile uploaded successfully for open use \n\n");
			}

			system("rm uploadfilepath");

			break;

		case '2': write(sockid,&choice,1);

			filename=(char*)malloc(sizeof(char)*20);

			printf("\nEnter the name of the file to be downloaded : ");

			scanf("%s",filename);

			i=0;

			while(1)
			{
				write(sockid,(filename+i),1);
				
				if(*(filename+i)=='\0')
				{
					break;
				}
				
				i++;
			}

			read(sockid,&dack,sizeof(int));

			if(dack==-1)
			{
				printf("\nFile is not available on the server \n");
				
				break;
			}

			read(sockid,&dwnsec,sizeof(int));

			if(dwnsec==1)
			{
				printf("\nFile is available on the server with security \n");

				key=(char*)malloc(sizeof(char)*20);

				printf("\nEnter the key to download the file : ");
				
				scanf("%s",key);

				i=0;

				while(1)
				{
					write(sockid,(key+i),1);
				
					if(*(key+i)=='\0')
					{
						break;
					}
				
					i++;
				}

				read(sockid,&kack,sizeof(int));

				if(kack==1)
				{
					printf("\nKey entered is correct \n");
				}

				else if(kack==0)
				{
					printf("\nKey entered is not correct \n");

					goto OUT;
				}
			}

			else
			{
				printf("\nFile is available on the server for free download \n");
			}
			
			dfd=open(filename,O_WRONLY|O_CREAT);

			while(1)
			{
				read(sockid,&count,sizeof(int));

				if(count==0)
				{
					break;
				}

				read(sockid,&ch,1);

				write(dfd,&ch,1);		
			}

			printf("\nFile %s downloaded from the server successfully \n\n",filename);

			ret=fork();
			
			if(ret==0)
			{
      				execl("movefiles","movefiles",filename,NULL);
			}
			
			wait(&stop);

			break;

		default: printf("\nNot a valid choice \n");

			 break;
	}

	return 0;
OUT:
	return -1;
}
