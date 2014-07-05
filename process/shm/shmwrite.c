#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
	char name[8];
	int age;
}people;

int main()
{
	int shm_id;
	int i;
	key_t key;
	char temp[8];
	people *p_map;
	char pathname[30];
	strcpy(pathname,"/tmp");

	key=ftok(pathname,0x33);
	if(-1==key)
	{
		perror("ftok error");
		exit(-1);
	}

	printf("key=[%d]\n",key);
	shm_id=shmget(key,4096,0644|IPC_CREAT|IPC_EXCL);
	if(-1==shm_id)
	{
		perror("shmget error");
		exit(-1);
	}

	printf("shm_id=[%d]\n",shm_id);

	p_map=(people *)shmat(shm_id,NULL,0);
	if((void *)(-1)==p_map)
	{
		perror("shmat error");
		exit(-1);
	}
	
	memset(temp,0,sizeof(temp));
	strcpy(temp,"test");
	temp[4]='0';

	for(i=0;i<3;i++)
	{
		temp[4]+=1;
		strncpy((p_map+i)->name,temp,5);
		(p_map+i)->age=0+i;
	}

	if(-1==shmdt(p_map))
	{
		perror("shmdt error");
		exit(-1);
	}

	return 0;
}	


