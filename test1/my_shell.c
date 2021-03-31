#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void print_shell()
{
    printf("shell $$");
};
void get_command(char *buf)
{
    memset(buf,'\0',256);
    char ch;
    int count=0;
    while ((ch=getchar())&&ch!='\n')
    {
        buf[count++]=ch;
        if (count==256)
        {
            printf("error:command is too long");
            exit(-1);
        }
    }
    buf[count]='\n';
}
void explain_command(const char *buf,char *count,char command[100][256])
{
    
}
void do_command()
{

}
int main()
{
    char command[100][256];
    int i=0;
    for (i=0;i<100;i++)
    {
        command[i][0]='\0';
    }

    int count;
    while (1)
    {
        print_shell();
        char *buf;
        buf=(char *)malloc(sizeof(char)*256);
        get_command(buf);
        if (strcmp(buf,'exit')==0||strcmp(buf,'logout')==0)
        {
            exit(0);
        }
        else explain_command(buf,&count,command);
        
    }
    
}