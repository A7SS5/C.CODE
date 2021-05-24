#include<stdio.h>
#include <errno.h>
#include<pthread.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
void my_error(const char *error_string, int line);
void add_func(void *(*func)(void *arg),void *arg);
void *mainfun(void *arg);
void init_threadpool(int max);
void destroypool();
void *simplework();


void my_error(const char *error_string, int line)
{
    fprintf(stderr, "line:%d",line);
    perror(error_string);
    
}
struct work_list{
    void *(*func)(void *arg);
    void *arg;
    struct work_list *next;
};
struct pthread_pool{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int max_threads;
    int work_num;
    int isdestroy;
    pthread_t* thread;
    struct work_list *list_head;
};
struct pthread_pool* pool;
void add_func(void *(*func)(void *arg),void *arg)
{
    struct work_list *a;
    a=(struct work_list *)malloc(sizeof(struct work_list));
    a->next=NULL;
    a->func=func;
    a->arg=arg;
    if (pool->list_head==NULL)
    {
        pool->list_head=a;
    }
    else {
        struct work_list *p;
        p=pool->list_head;
        while(p->next!=NULL)
        {
            p=p->next;
        }
        p->next=a;
    }
    pool->work_num++;
    pthread_mutex_unlock(&(pool->mutex));
    pthread_cond_signal(&(pool->cond));
    printf("try to awake\n");
}
void *mainfun(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&(pool->mutex));
        while(pool->list_head==NULL&&pool->work_num==0)
        {
            if (pool->isdestroy==1)
            {
   //             pthread_mutex_unlock(&(pool->mutex));
                printf("pthread %d has destroyed\n",pthread_self());
                pthread_exit(NULL);
            }
            else {
                printf("%d is waiting\n",pthread_self());
                pthread_cond_wait(&(pool->cond),&(pool->mutex));
                printf("%d is awake\n",pthread_self());
            }
            
        }

        struct work_list *work=pool->list_head;
        pool->list_head=pool->list_head->next;
        pool->work_num--;
        pthread_mutex_unlock(&(pool->mutex));
        (*(work->func)) (work->arg);
        free(work);
        work=NULL;
    }
    pthread_exit(NULL);
}
void init_threadpool(int max)
{
    pool=(struct pthread_pool *)malloc(sizeof(struct pthread_pool));
    if (pool == NULL)
    {
        my_error("malloc",__LINE__);
    }
    pool->max_threads=max;
    pool->work_num=0;
    pool->isdestroy=0;
    pool->list_head=NULL;
    pthread_mutex_init(&(pool->mutex),NULL);
    pthread_cond_init(&(pool->cond),NULL);
    int i=0;
    pool->thread=(pthread_t*)malloc(max*sizeof(pthread_t));
    if (pool->thread == NULL)
    {
        my_error("malloc",__LINE__);
    }
    int ret;
    for (i=0;i<max;i++)
    {
        ret=pthread_create(&(pool->thread[i]),NULL,mainfun,NULL);
        if (ret!=0)
        {
            my_error("pthread_create",__LINE__);
        }
    }
    printf("init is sucess\n");
}
void destroypool()
{
    pthread_mutex_lock(&(pool->mutex));
    pool->isdestroy=1;
    pthread_mutex_unlock(&(pool->mutex));
  pthread_cond_broadcast(&(pool->cond));
    printf("i call everyone\n");
    int i=0;
    for (i=0;i<pool->max_threads;i++)
    {
        pthread_join(pool->thread[i],NULL);
    }
    free(pool->thread);
    pthread_cond_destroy(&(pool->cond));
    pthread_mutex_destroy(&(pool->mutex));
    free(pool);
}
void *simplework(void *arg)
{
    printf("%d is working %d\n",pthread_self(),*(int*)arg);
    sleep(1);
}
int main()
{
    init_threadpool(10);
    int i=0;
    for (i=0;i<5;i++)
    {
        pthread_mutex_lock(&(pool->mutex));
        add_func(simplework,&i);
    }
    printf("%d work is inline now\n",i);
    sleep(5);
    destroypool();
    printf("destory compelete\n");
    return 0;
}