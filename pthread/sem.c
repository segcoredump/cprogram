#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sched.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

sem_t tsem;
char str[20]="hello everyone";

void *mutex_read_1(void *arg)
{
	int ret=0;
	int sval;
	struct timespec t;
	t.tv_sec=time(NULL);
	t.tv_sec+=5;
	t.tv_nsec=0;

	sem_getvalue(&tsem,&sval);
	printf("sval=[%d]\n",sval);
	ret=sem_wait(&tsem);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_mutex_lock",strerror(ret));
	}

	usleep(200);
	printf("read_1=[%s]\n",str);
	sem_post(&tsem);

	return NULL;
}

void *mutex_read_2(void *arg)
{
	int ret=0;
	ret=sem_wait(&tsem);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_mutex_lock",strerror(ret));
	}

	usleep(200);
	printf("read_2=[%s]\n",str);

	sem_post(&tsem);

	return NULL;
}

void *mutex_write_1(void *arg)
{
	int ret=0;
	ret=sem_wait(&tsem);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_mutex_lock",strerror(ret));
	}
	strcpy(str,"change_1");
	printf("write_1=[%s]\n",str);
	sem_post(&tsem);

	
	return NULL;

}

void *mutex_write_2(void *arg)
{
	int ret=0;
	ret=sem_wait(&tsem);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_mutex_lock",strerror(ret));
	}
	strcpy(str,"change_2");
	printf("write_2=[%s]\n",str);
	sem_post(&tsem);


	return NULL;
}

int main()
{
	pthread_t tid[6];
	int ret;
	int i;

	ret=sem_init(&tsem,0,2);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","sem_init",strerror(ret));
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

	sem_destroy(&tsem);

	return 0;
}	
