#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

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

	shm_id=shmget(key,0,0);
	if(-1==shm_id)
	{
		perror("shmget error");
		exit(-1);
	}

	printf("shm_id=[%d]\n",shm_id);
	p_map=(people *)shmat(shm_id,NULL,0);
	for(i=0;i<3;i++)
	{
		printf("name=[%s]\n",(p_map+i)->name);
		printf("age=[%d]\n",(p_map+i)->age);
	}

	if(-1==shmdt(p_map))
	{
		perror("shmdt error");
		exit(-1);
	}

	if(-1==shmctl(shm_id,IPC_RMID,NULL))
	{
		perror("shmctl error");
		exit(-1);
	}


	return 0;
}


