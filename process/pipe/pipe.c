#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int fd[2];
	int ret;
	char buff[40];


	if(pipe(fd)<0)
	{
		perror("create pipe");
		exit(-1);
	}

	ret=fork();
	if(ret<0)
	{
		perror("fork");
		exit(-1);
	}
	else if(0==ret)
	{
		printf("The child process\n");
		close(fd[1]);
		memset(buff,0,sizeof(buff));
		sleep(2);

		if(-1==read(fd[0],buff,sizeof(buff)))
		{
			perror("read");
			exit(-1);
		}

		printf("buff=[%s]\n",buff);
		close(fd[0]);

		return 0;
	}
	else
	{
		printf("The parent process\n");
		close(fd[0]);
		if(-1==write(fd[1],"hello ",6))
		{
			perror("pipe fd write");
			exit(-1);
		}

		if(-1==write(fd[1],"The parent call",15))
		{
			perror("pipe fd write");
			exit(-1);
		}

		close(fd[1]);

		sleep(3);
		waitpid(ret,NULL,0);

		return 0;
	}
	
}
