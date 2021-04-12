#include<stdio.h>
#include<stdlib.h>
struct Node {
    int data;
    struct Node*next; 
};
typedef struct Node* LinkedList;
void my_error(const char *error_string, int line)
{
    fprintf(stderr, "line:%d",line);
    perror(error_string);    
}
void createnewList(LinkedList* L)
{
    *L=(LinkedList)malloc(sizeof(LinkedList));
    if (*L==NULL)
    {
        my_error("malloc",__LINE__);
        return;
    }
    else (*L)->next=NULL;
}
void addnewdata(LinkedList L,int data)
{
    LinkedList p=(LinkedList)malloc(sizeof(LinkedList));
    if (p==NULL)
    {
        my_error("malloc",__LINE__);
        return;
    }
    p->next=NULL;
    p->data=data;
    if (L->next==NULL)
    {
        L->next=p;
        p->next=NULL;
    }
    else{
        p->next=L->next;
        L->next=p;
    }
}
void printall(LinkedList L)
{
    LinkedList p=L->next;
    while(p)
    {
        printf("this is data:%d\n",p->data);
        p=p->next;
    }
}
void freeall(LinkedList* L)
{
    LinkedList p=*L,q=NULL;
    if (p->next==NULL)
    {
        free(L);
        return;
    }
    p=p->next;
    q=p->next;
    while(p)
    {
        q=p;
        p=q->next;
        free(q);
    }
    free(*L);
}
int main()
{
    LinkedList L=NULL;
    createnewList(&L);
    addnewdata(L,1);
    addnewdata(L,2);
    printall(L);
    freeall(&L);
    printall(L);
}