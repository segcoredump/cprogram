#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

struct mmsgbuf
{
	long mtype;
	char mtext[10];
};

int main()
{
	int msqid;
	int ret;
	char buff[10];
	key_t MSGKEY=2049;
	struct mmsgbuf stMSG;

	msqid=msgget(MSGKEY,0666 | IPC_CREAT);
	if(-1==msqid)
	{
		perror("IPC_CREAT");
		exit(-1);
	}

	while(1)
	{
		printf("enter a string:\n");
		fgets(buff,sizeof(buff),stdin);
		stMSG.mtype=1;
		strcpy(stMSG.mtext,buff);
		ret=msgsnd(msqid,&stMSG,sizeof(struct mmsgbuf),IPC_NOWAIT);
		if(-1==ret)
		{
			perror("msgsnd");
			exit(-1);
		}

		if(0==strncmp(stMSG.mtext,"end",3))
			break;
	}

	return 0;
}
