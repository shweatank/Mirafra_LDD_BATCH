#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#define SIZE 4
#define FILEPATH  "/dev/key_value"
struct data
{
	int key;
	int value;
};

int main()
{

	struct data set[SIZE];
        int *ptr;
	int key;
	int fd = open(FILEPATH,O_WRONLY);
	perror("open");
	for(int i = 0; i< SIZE ;i++)
	{
		scanf("%d %d",&set[i].key,&set[i].value);
	}
	write(fd,set,sizeof(set));
	perror("write");
	close(fd);
        while(1)
	{	
        printf("enter the key\n");
	scanf("%d",&key);
	ptr = &key;
	fd = open(FILEPATH,O_RDONLY);
	read(fd,ptr,sizeof(*ptr));
	perror("read");
	printf("data = %d\n",*ptr);
	}
}
