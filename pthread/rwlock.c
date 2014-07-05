#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <stdlib.h>

pthread_rwlock_t trwlock;
int pshared;
char str[20]="hello everyone";

void *rwlock_read_1(void *arg)
{
	int ret;
	ret=pthread_rwlock_rdlock(&trwlock);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_rwlock_rdlock",strerror(ret));
	}
	usleep(200);
	printf("str=[%s]\n",str);
	pthread_rwlock_unlock(&trwlock);

	return NULL;
}

void *rwlock_read_2(void *arg)
{
	int ret;
	ret=pthread_rwlock_rdlock(&trwlock);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_rwlock_rdlock",strerror(ret));
	}
	usleep(200);
	printf("str=[%s]\n",str);
	pthread_rwlock_unlock(&trwlock);
	return NULL;

}

void *rwlock_write_1(void *arg)
{
	int ret;
	ret=pthread_rwlock_wrlock(&trwlock);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_rwlock_wrlock",strerror(ret));
	}
	strcpy(str,"change_1");
	printf("str=[%s]\n",str);
	pthread_rwlock_unlock(&trwlock);
	
	return NULL;

}

void *rwlock_write_2(void *arg)
{
	int ret;
	ret=pthread_rwlock_wrlock(&trwlock);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_rwlock_wrlock",strerror(ret));
	}
	strcpy(str,"change_2");
	printf("str=[%s]\n",str);
	pthread_rwlock_unlock(&trwlock);

	return NULL;
}

int main()
{
	pthread_t tid[6];
	int ret;
	int i;

	ret=pthread_rwlock_init(&trwlock,NULL);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_rwlock_init",strerror(ret));
		exit(-1);
	}


	ret=pthread_create(&tid[0],NULL,rwlock_read_1,NULL);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_create",strerror(ret));
		exit(-1);
	}

	ret=pthread_create(&tid[1],NULL,rwlock_read_2,NULL);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_create",strerror(ret));
		exit(-1);
	}

	ret=pthread_create(&tid[2],NULL,rwlock_write_1,NULL);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_create",strerror(ret));
		exit(-1);
	}

	ret=pthread_create(&tid[3],NULL,rwlock_write_2,NULL);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_create",strerror(ret));
		exit(-1);
	}

	for(i=0;i<4;i++)
	{
		ret=pthread_join(tid[i],NULL);
		if(0!=ret)
		{
			fprintf(stderr,"%s:%s\n","pthread_join",strerror(ret));
			exit(-1);
		}
	}
	pthread_rwlock_destroy(&trwlock);

	return 0;
}	
