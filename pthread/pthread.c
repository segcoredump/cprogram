#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <sys/types.h>

#define NDEATCH
pthread_once_t tonce=PTHREAD_ONCE_INIT;
pthread_key_t tkey;

void *cancel_test(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
	usleep(500);
	pthread_testcancel();
	
	return 0;
}
	

void *pthread_key(void *arg)
{
	usleep(500);
	char str[20]="pthread_key test";
	char *tmp;
	pthread_setspecific(tkey,(void *)str);
	tmp=(char *)pthread_getspecific(tkey);
	printf("pthread key=[%s]\n",tmp);

    return 0;
}

void run_once()
{
	usleep(500);
	fprintf(stdout,"[%s]\n","run once");
}

void *pthread_id(void *arg)
{
	usleep(500);
	fprintf(stdout,"id=[%lu]\n",pthread_self());

	return 0;
}

int main()
{
	pthread_t tid[6];
	pthread_attr_t tattr;

    void *(*ppfunc)(void *arg);
	int ret;
	int scope;
	int policy=SCHED_RR;
	struct sched_param param;
	int detachstate;
	int level=4;
	int i;

	param.__sched_priority=6;

	pthread_key_create(&tkey,NULL);

	pthread_attr_init(&tattr);
    #ifdef DEATCH
	    pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);
    #endif    
	pthread_attr_setscope(&tattr,PTHREAD_SCOPE_PROCESS);
	pthread_attr_setschedpolicy(&tattr,SCHED_RR);
	pthread_attr_setschedparam(&tattr,&param);
	pthread_setconcurrency(level);
	

	ret=pthread_create(&tid[5],&tattr,cancel_test,NULL);
	pthread_setschedparam(tid[5],policy,&param);
	usleep(100);
	pthread_getschedparam(tid[5],&policy,&param);
	printf("policy=[%d] sched_priority=[%d] \n",policy,param.__sched_priority);
	pthread_cancel(tid[5]);
	if(0!=ret)
	{
		fprintf(stderr,"%s:%s\n","pthread_create",strerror(ret));
		exit(-1);
	}
	usleep(100);

	pthread_once(&tonce,run_once);
	for(i=0;i<4;i++)
	{	
        ppfunc=(i==3)?cancel_test:pthread_id;
        ppfunc=(i==2)?pthread_key:pthread_id;
		ret=pthread_create(&tid[i],&tattr,ppfunc,NULL);
		if(0!=ret)
		{
			fprintf(stderr,"%s:%s\n","pthread_create",strerror(ret));
			exit(-1);
		}
        
        if(3==i)
	        pthread_cancel(tid[i]);
        #ifdef DEATCH
		pthread_attr_getdetachstate(&tattr,&detachstate);
		fprintf(stdout,"detachstate=[%d]\n",detachstate);
        #endif
		//pthread_detach(tid[i]);
	}

	pthread_attr_getdetachstate(&tattr,&detachstate);
	printf("detachstate=[%d]\n",detachstate);
	pthread_attr_getscope(&tattr,&scope);
	printf("scope=[%d]\n",scope);
	pthread_attr_getschedpolicy(&tattr,&policy);
	printf("policy=[%d]\n",policy);
	pthread_attr_getschedparam(&tattr,&param);
	printf("param.__sched_priority=[%d]\n",param.__sched_priority);

	level=0;
	level=pthread_getconcurrency();
	printf("level=[%d]\n",level);
	
	ret=pthread_equal(tid[0],tid[1]);
	if(0==ret)
		printf("[%lu] not equal [%lu]\n",tid[0],tid[1]);
	else
		printf("[%lu] equal [%lu]\n",tid[0],tid[1]);
		
	//pthread_kill(tid[0],SIGKILL);
    #ifdef NDEATCH 
        for(i=0;i<3;i++)
        {
            ret=pthread_join(tid[i],NULL);
            if(0!=ret)
            {
                fprintf(stderr,"%s:%s\n","pthread_join",strerror(ret));
                exit(-1);
            }
        }
    #endif 

	pthread_attr_destroy(&tattr);
	return 0;
}
