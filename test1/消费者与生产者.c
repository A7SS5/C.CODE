#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<pthread.h>
void my_error(const char *error_string, int line)
{
    fprintf(stderr, "line:%d",line);
    perror(error_string);
    
}
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  //静态初始化
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  //静态初始化
struct node {
    struct node* next;
    int data;
};
struct node *head;
void *consumer(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        pthread_t tid;
        tid=pthread_self();
    struct node *out;
        while (head == NULL)
        {
            pthread_cond_wait(&cond, &mutex);
        }
    out=head;
    head=head->next; 
    printf("consumer : %lu has get %d\n",tid,out->data);
    free(out);
    pthread_mutex_unlock(&mutex);
    sleep(rand()%3);
    }
    return NULL;
}
void *producer(void *arg)
{
    while(1)
    {
        pthread_t tid;
        tid=pthread_self();
        struct node *out=(struct node *)malloc(sizeof(struct node));
        out->data=rand()%1000+1;
        pthread_mutex_lock(&mutex);
        out->next=head;
        head=out;
        printf("producer : %lu has made %d\n",tid,out->data);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        sleep(rand()%3);
    }
    return NULL;
}
int main()
{
    pthread_t pid,pid1,cid,cid1;
    int tmp;
    srand(time(NULL));
    if ((tmp=pthread_create(&pid,NULL,consumer,NULL))!=0)
    {
        my_error("pthread_cread",__LINE__);
    }
    if ((tmp=pthread_create(&pid1,NULL,consumer,NULL))!=0)
    {
        my_error("pthread_cread",__LINE__);
    }
    if ((tmp=pthread_create(&cid,NULL,producer,NULL))!=0)
    {
        my_error("pthread_cread",__LINE__);
    }if ((tmp=pthread_create(&cid1,NULL,producer,NULL))!=0)
    {
        my_error("pthread_cread",__LINE__);
    }
    pthread_join(pid,NULL);
    pthread_join(pid1,NULL);
    pthread_join(cid,NULL);
    pthread_join(cid1,NULL);
    return 0;
}