#include<sys/types.h>
#include<stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include<stdio.h>
#include <unistd.h>
void my_error(const char *error_string, int line)
{
    fprintf(stderr, "line:%d",line);
    perror(error_string);
    
}
int main()
{
    key_t key;
   union semun {
        int val;
 //       struct semid_ds *buf;
        ushort *array;
        struct seminfo *buf;
        void *pad;    
    }semopts;
    int semid,semval;
    if ((key=ftok(".",'s'))==-1)
    {
        my_error("ftok",__LINE__);
        exit(1);
    }

    if ((semid=semget(key,1,IPC_CREAT|0666))==-1)
    {
        my_error("semget",__LINE__);
        exit(1);
    }

    while(1)
    {
        if ((semval=semctl(semid,0,GETVAL))==-1)
        {
            my_error("semctl",__LINE__);
            exit(1);
        }
        if (semval>0)
        {
            printf("still %d resources can be used\n",semval);
        }
        else
        {
            printf("no more resources can bu used\n");
            break;
        }
        sleep(3);
    }
    exit(0);
}