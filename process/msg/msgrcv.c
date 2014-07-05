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
	char str[10];
	key_t MSGKEY=2049;
	struct mmsgbuf stMSG;

	msqid=msgget(MSGKEY,0666 | IPC_CREAT);
	if(-1==msqid)
	{
		perror("msgrecive [msgget]");
		exit(-1);
	}
	
	while(1)
	{
		ret=msgrcv(msqid,&stMSG,sizeof(struct mmsgbuf),0,0);
		if(-1==ret)
		{
			perror("msgrcv");
			exit(-1);
		}

		if(0==strncmp(stMSG.mtext,"end",3))
			break;

		strncpy(str,stMSG.mtext,10);
		printf("mtext=[%s]\n",str);
	}

	ret=msgctl(msqid,IPC_RMID,0);
	if(-1==ret)
	{
		perror("msgctl");
		exit(-1);
	}

	return 0;
}
