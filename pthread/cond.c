#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t tmutex;
pthread_cond_t tcond;
char str[20]="hello everyone";
int icond=1;

void *mutex_read_1(void *arg)
{
	int ret;
	struct timespec t;
	t.tv_sec=time(NULL);
	t.tv_sec+=5;
	t.tv_nsec=0;
	ret=pthread_mutex_lock(&tmutex);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_mutex_lock",strerror(ret));
	}
	if(1==icond)
	{
		printf("[%s]\n","cond delay");
		pthread_cond_timedwait(&tcond,&tmutex,&t);
	}

	usleep(200);
	printf("read_1=[%s]\n",str);
	pthread_mutex_unlock(&tmutex);

	return NULL;
}

void *mutex_read_2(void *arg)
{
	int ret;
	ret=pthread_mutex_lock(&tmutex);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_mutex_lock",strerror(ret));
	}

	if(1==icond)
	{
		icond=0;
		printf("[%s]\n","cond signal");
		pthread_cond_signal(&tcond);
	}

	usleep(200);
	printf("read_2=[%s]\n",str);
	pthread_mutex_unlock(&tmutex);

	return NULL;
}

void *mutex_write_1(void *arg)
{
	int ret;
	ret=pthread_mutex_lock(&tmutex);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_mutex_lock",strerror(ret));
	}
	strcpy(str,"change_1");
	printf("write_1=[%s]\n",str);
	pthread_mutex_unlock(&tmutex);
	
	return NULL;

}

void *mutex_write_2(void *arg)
{
	int ret;
	ret=pthread_mutex_lock(&tmutex);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_mutex_lock",strerror(ret));
	}
	strcpy(str,"change_2");
	printf("write_2=[%s]\n",str);
	pthread_mutex_unlock(&tmutex);

	return NULL;
}

int main()
{
	pthread_t tid[6];
	int ret;
	int i;

	pthread_mutexattr_t tmattr;

	pthread_mutexattr_init(&tmattr);
	pthread_mutexattr_settype(&tmattr,PTHREAD_MUTEX_RECURSIVE);

	ret=pthread_mutex_init(&tmutex,&tmattr);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_mutex_init",strerror(ret));
		exit(-1);
	}

	pthread_cond_init(&tcond,NULL);

	ret=pthread_create(&tid[0],NULL,mutex_read_1,NULL);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_create",strerror(ret));
		exit(-1);
	}

	ret=pthread_create(&tid[1],NULL,mutex_read_2,NULL);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_create",strerror(ret));
		exit(-1);
	}

	ret=pthread_create(&tid[2],NULL,mutex_write_1,NULL);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_create",strerror(ret));
		exit(-1);
	}

	ret=pthread_create(&tid[3],NULL,mutex_write_2,NULL);
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
	pthread_cond_destroy(&tcond);
	pthread_mutexattr_destroy(&tmattr);
	pthread_mutex_destroy(&tmutex);

	return 0;
}	
