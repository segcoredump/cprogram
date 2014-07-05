#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <stdlib.h>

pthread_mutex_t tmutex;
char str[20]="hello everyone";

void *mutex_read_1(void *arg)
{
	int ret;
	int i;
	ret=pthread_mutex_lock(&tmutex);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_mutex_lock",strerror(ret));
	}
	usleep(200);
	printf("[%s]\n","mutex try lock six");
	for(i=0;i<6;i++)
	{	
		ret=pthread_mutex_lock(&tmutex);
		if(0!=ret)
		{
			fprintf(stderr,"%s:%s\n","pthread_mutex_lock",strerror(ret));
			exit(-1);
		}
	}
	printf("[%s]\n","mutex locked six");
	printf("[%s]\n","mutex try unlock five");
	for(i=0;i<5;i++)
	{	
		ret=pthread_mutex_unlock(&tmutex);
		if(0!=ret)
		{
			fprintf(stderr,"%s:%s\n","pthread_mutex_lock",strerror(ret));
			exit(-1);
		}
	}
	printf("[%s]\n","mutex unlocked five");
	usleep(200);
	printf("str=[%s]\n",str);
	pthread_mutex_unlock(&tmutex);
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
	usleep(200);
	printf("str=[%s]\n",str);
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
	printf("str=[%s]\n",str);
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
	printf("str=[%s]\n",str);
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
	pthread_mutexattr_destroy(&tmattr);
	pthread_mutex_destroy(&tmutex);

	return 0;
}	
