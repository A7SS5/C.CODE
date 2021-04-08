#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
void my_error(const char *error_string, int line);
void print_shell();
void get_command(char *buf);
void explain_command(const char *buf,int *count,char command[100][256]);
void do_command(char command[100][256],int count);
int find_function(char* command);
void print_shell()
{
    printf("\nshell $$");
};
void my_error(const char *error_string, int line)
{
    fprintf(stderr, "line:%d",line);
    perror(error_string);
    
}
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
void explain_command(const char *buf,int *count,char command[100][256])
{
    int i=0;
    int j=0;
    while(buf[i]!='\n')
    {
        if (buf[i]==' ')
        {
            command[*count][j]='\0';

            *count=*count+1;
            j=0;
            i++;
        }
        command[*count][j++]=buf[i];
        i++;
    }
    command[*count][j]='\0';
    *count=*count+1;

}
void do_command(char command[100][256],int count)
{
    char* arg[count+1];
    pid_t pid;
    char* argnext[count+1];
    int i=0;
    int key=0;
    int flag=0;
    int fd;
    int fd2;
    pid_t pid2;
    int status;
    int status2;
    int j=0;
    char *file;
    for (i=0;i<count;i++)
    {
        arg[i]=(char*)command[i];
    }
    arg[count]=NULL;
    for (i= 0; i < count; i++)
    {
        if (strcmp(arg[i],">")==0)
        {
            flag++;
            key=1;
            if(arg[i+1]==NULL)
            {
                flag++;
            }
        }
        if (strcmp(arg[i],"<")==0)
        {
            flag++;
            key=2;
            if(arg[i+1]==NULL||i==0)
            {
                flag++;
            }
        }
        if (strcmp(arg[i],"|")==0)
        {
            flag++;
            key=3;
            if(arg[i+1]==NULL||i==0)
            {
                flag++;
            }
        }
    
    }
    if (flag>1)
    {
        printf("%d",flag);
        printf("commond is not correct");
        return;
    }
    if (key==1||key==2)
    {   
        for (i= 0; i < count; i++)
        {
            if (strcmp(arg[i],">")==0||strcmp(arg[i],"<")==0)
            {
                file=arg[i+1];
                arg[i]=NULL;
                break;
            }
            
        }
    }
    if (key==3)
    {
        for (i= 0; i < count; i++)
        {
            if (strcmp(arg[i],"|")==0)
            {
               arg[i]=NULL;
               j=0;
                i++;
                while(arg[i]!=NULL)
                {
                    argnext[j++]=arg[i++];
                }
                argnext[j]=arg[i];
                break;
            }
            
        }
    }
    if ((pid=fork())<0)
    {
        my_error("fork",__LINE__);
        return;
    }

    switch(key)
    {   
        case 0:
        if (pid==0)
        {
            if (find_function(command[0])!=1)
            {
                printf("command not found");
                exit(0); 
            }
            execvp(arg[0],arg);
            exit(0);
        }
        break;
        case 1: 
        if (pid==0)
        {
            if (find_function(command[0])!=1)
            {
                printf("command not found");
                exit(0); 
            }
            fd=open(file,O_RDWR|O_CREAT|O_TRUNC,0664);
            dup2(fd,1);
            execvp(arg[0],arg);
            exit(0);
        }
        break;
        case 2:
        if (pid==0)
        {
            if (find_function(command[0])!=1)
            {
                printf("command not found");
                exit(0); 
            }
            fd=open(file,O_RDONLY);
            dup2(fd,0);
            execvp(arg[0],arg);
            exit(0);
        }
        break;
        case 3:
        if (pid==0)
        {
            if ((pid2=fork())<0)
            {
                my_error("fork",__LINE__);
                return;
            }
            else if (pid2==0)
            {
                if (find_function(command[0])!=1)
                {
                    printf("command not found");
                    exit(0);  
                }

                fd2=open("/tmp/templefile",O_RDWR|O_CREAT|O_TRUNC,0664);
                dup2(fd2,1);
                execvp(arg[0],arg);
                exit(0);
            }
        
            if (waitpid(pid2,&status2,0)==-1)
            {
                my_error("waitpid",__LINE__);
            }
            
            if (find_function(argnext[0])!=1)
                {
                    printf("command not found");
                    exit(0);  
                }

            fd2=open("/tmp/templefile",O_RDONLY);
            dup2(fd2,0);
            execvp(argnext[0],argnext);
          //  if (remove("/tmp/templefile"))
            my_error("remove",__LINE__);
            exit(0);
        
        }

        break;
        default:
        break;
    }
    if (waitpid(pid,&status,0)==-1)
    my_error("waitpid",__LINE__);
}
int find_function(char* command)
{
    char en[][256]={"./","/bin","/usr/bin"};
    int i=0;
    DIR *dir;
    struct dirent *ptr;
    for (i=0;i<3;i++)
    {
        if ((dir=opendir(en[i]))==NULL)
        {
            my_error("opendir",__LINE__);
            return -1;
        }
        while((ptr = readdir(dir)) != NULL)
        {
            if (strcmp(ptr->d_name,command)==0)
            {
                closedir(dir);
                return 1;
            }
        }
        closedir(dir);
    }
    return 0;
}
int main()
{
    char command[100][256];
    int i=0;
    for (i=0;i<100;i++)
    {
        command[i][0]='\0';
    }

    
    while (1)
    {
        int count=0;
        print_shell();
        char *buf;
        buf=(char *)malloc(sizeof(char)*256);
        get_command(buf);
        if ((strcmp(buf,"exit\n")==0)||(strcmp(buf,"logout\n")==0))
        {
            exit(0);
        }
        else explain_command(buf,&count,command);
        do_command(command,count);
        free(buf);
    }
}