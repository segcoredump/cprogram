#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define FIFO_NAME "pipe.txt"

int main()
{
	int pipe_fd;
	int ret;
	char buff[20];

	if(-1==access(FIFO_NAME,F_OK))
	{
		ret=mkfifo(FIFO_NAME,0766);
		if(0!=ret)
		{
			perror("mkfifo");
			exit(-1);
		}
	}
			
	pipe_fd=open(FIFO_NAME,O_WRONLY);
	if(-1==pipe_fd)
	{
		perror("open");
		exit(-1);
	}

	memset(buff,0,sizeof(buff));
	strcpy(buff,"hello,everyone");
	ret=write(pipe_fd,buff,sizeof(buff));
	if(-1==ret)
	{
		perror("write");
		exit(-1);
	}

	close(pipe_fd);

	return 0;
}
