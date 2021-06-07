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
    int semid;
    struct sembuf sbuf={0,-1,IPC_NOWAIT};
    union semun {
        int val;
 //       struct semid_ds *buf;
        ushort *array;
        struct seminfo *buf;
        void *pad;    
    }semopts;
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

    semopts.val=5;
    if ((semctl(semid,0,SETVAL,semopts))==-1)
    {
        my_error("semctl",__LINE__);
        exit(1);
    }

    while(1)
    {
        if ((semop(semid,&sbuf,1))==-1)
        {
            my_error("semop",'s');
            exit(1);
        }
        sleep(4);
    }
    exit(0);
}