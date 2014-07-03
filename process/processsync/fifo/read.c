#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_NAME "pipe.txt"

int main()
{
	int pipe_fd;
	char buff[20];
	int rdbytes;
	
	pipe_fd=open(FIFO_NAME,O_RDONLY);
	if(-1==pipe_fd)
	{
		perror("open");
		exit(-1);
	}

	rdbytes=read(pipe_fd,buff,sizeof(buff));
	printf("read bytes [%d],string is [%s]\n",rdbytes,buff);

	close(pipe_fd);

	return 0;
}
