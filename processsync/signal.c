#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

#define NDEBUG
#define pMsg(fmt...) fprintf(stdout,##fmt)

void signal_int(int sig)
{
	pMsg("catch sigint signal\n");
	exit(0);
}

void test_signal(int isig)
{
	struct itimerval otimer;
	switch(isig)
	{
		case SIGALRM:
			pMsg("catch sigalrm signal\n");
			#ifdef DEBUG
				getitimer(ITIMER_REAL,&otimer);
				pMsg("sigalrm interval mircroseconds=[%ld]\n",otimer.it_interval.tv_sec*1000000+otimer.it_interval.tv_usec);
			#endif
			signal(SIGALRM,test_signal);

			break;
		case SIGVTALRM:
			pMsg("catch sigvtalrm signal\n"); 
			#ifdef DEBUG
				getitimer(ITIMER_VIRTUAL,&otimer);
				pMsg("sigvtalrm interval microseconds=[%ld]\n",otimer.it_interval.tv_sec*1000000+otimer.it_interval.tv_usec);
			#endif
			signal(SIGVTALRM,test_signal);
				
			break;
		case SIGUSR1:
			pMsg("catch sigusr1 signal\n");
			signal(SIGUSR1,test_signal);

			break;
	}
	return;
}


int main( void )
{
	pid_t tpid;
	struct itimerval ftimer,otimer,stimer;
	ftimer.it_interval.tv_sec=1;
	ftimer.it_interval.tv_usec=0;
	ftimer.it_value.tv_sec=1;
	ftimer.it_value.tv_usec=0;

	stimer.it_interval.tv_sec=0;
	stimer.it_interval.tv_usec=50000;
	stimer.it_value.tv_sec=0;
	stimer.it_value.tv_usec=50000;

	alarm(2);
	signal(SIGALRM,test_signal);
	signal(SIGVTALRM,test_signal);
	signal(SIGUSR1,test_signal);

	setitimer(ITIMER_REAL,&ftimer,&otimer);	
	setitimer(ITIMER_VIRTUAL,&stimer,&otimer);	

	tpid=getpid();
	for(;;)
	{
		sleep(1);
		kill(tpid,SIGUSR1);
		signal(SIGINT,signal_int);
	}


	return 0;

}
	
