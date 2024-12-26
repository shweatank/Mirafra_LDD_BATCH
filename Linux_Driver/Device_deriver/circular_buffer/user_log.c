/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

#define DEVICE_PATH "/dev/log_device"
#define max_size 5
struct log_msg
{
	char log_message[256];
};
int i=0,n;
int main()
{
	int x,y;
	struct log_msg s[5];
	char k_buffer[1024];
	int fd=open(DEVICE_PATH,0666);
	if(fd<0)
	{
		printf("failed to open");
		return 1;
	}
	while(1)
	{

				printf("enter the log msg:\n");
				scanf(" %[^\n]s",s[i].log_message);
				x=write(fd,&s[i].log_message,sizeof(struct log_msg));
				y=read(fd,k_buffer,sizeof(k_buffer)-1);
			//	printf("the num of bytes to read is %d\n",y);
				k_buffer[y]='\0';
				printf("the message read from kernel is : %s\n",k_buffer);
				i=(i+1)%sizeof(max_size);
	
	}
	return 0;
}

