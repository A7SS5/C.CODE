//只支持-l -a -R
#include<stdio.h>
#include <string.h>
#include<sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>
#include<time.h>
#include <grp.h>
#include <dirent.h>
#include <pwd.h>
#define MAX_SIZE 100;        //最大的行宽
int space=100;         //剩余空间
int max_long=0;        //每个目录下最长的文件名
void my_error(const char *error_string, int line)
{
    fprintf(stderr, "line:%d", line);
    perror(error_string);
    
}
void simple_play(char *name) //无参数只输出名字
{
    printf("%s",name);
    int len=strlen(name);
    int i=0;
    for (i=0;i<max_long-len;i++)
    {
        printf(" ");
    }
    printf("  ");
    space=space-max_long-2;
    
    if (max_long>space)
    {
        printf("\n");
        space=100;
    }
}
void display_type(char *name,int flag)
{
    
}
void work_dir(char *path,int flag)
{
    DIR *dir;
    int num=0;
    int i=0;
    struct dirent*ptr;
    if ((dir=opendir(path))==NULL)
    {
        my_error("opendir",__LINE__);
        return;
    }
    while ((ptr=readdir(dir))!=NULL)
    {
        num++;
        if ((strlen(ptr->d_name)>max_long))
        {
            max_long=strlen(ptr->d_name);
        }
    }
    closedir(dir);
    if ((dir=opendir(path))==NULL)
    {
        my_error("opendir",__LINE__);
        return;
    }
    char **filenames;
    int len=(strlen(path));
    filenames=(char**)malloc(sizeof(char*)*num);
    if (filenames==NULL)
    {
        my_error("malloc",__LINE__);
    }
    for (i=0;i<num;i++)
    {   if ((readdir(dir))==NULL)
        {
            break;
        }
        filenames[i]=(char*)malloc(sizeof(char)*(max_long+len+1));
        if (filenames[i]==NULL)
        {
        my_error("malloc",__LINE__);
        }
        strncpy(filenames[i],path,len);
        filenames[i][len]='\0';
        strcat(filenames[i],ptr->d_name);
    }
    for (i=0;i<num;i++)
    {
        simple_play(filenames[i]);
        free(filenames[i]);
    }
    free(filenames);
    closedir(dir);
    
}

int main(int argc, char **argv)
{
    char argustr[100];
    int count_arguments=0;
    struct stat buf;
    int i=1;
    for (i=1;i<argc;i++)
    {   
        if (argv[i][0]=='-')
        {
        count_arguments++;
        strcat(argustr, argv[i]);
        }
        
    }
    int flag=0;
//获取ls的参数，a=1,l=2,R=4;
    int l=strlen(argustr);
    int judge_a=0,judge_l=0,judge_R=0;
    for (i=0;i<l;i++)
    {
        if (argustr[i]=='-')
        {
            continue;
        }
        else if (argustr[i]=='a'&&judge_a==0)
        {
             flag++;
             judge_a=1;
        }
        else if (argustr[i]=='l'&&judge_l==0)
        {
             flag=flag+2;
             judge_a=1;
        }else if (argustr[i]=='R'&&judge_R==0)
        {
             flag=flag+4;
             judge_R=1;
        }
        else
        {
            printf("arguments don't support");
            exit(0);           
        }
    }
    char path[PATH_MAX];       //无参数就作用于当前目录。
    if (argc==(count_arguments+1))
    {
        strcpy(path,"./");
        path[2]='\0';
        work_dir(path,flag);
    }
    for (i=1;i<argc;i++)
    {
        if (argv[i][0]=='-')
        {
            continue;
        }
        if (lstat(argv[i],&buf)==-1)
        {
            my_error("lstat",__LINE__);
            continue;
        }
        if (S_ISDIR(buf.st_mode)) //是目录就先处理后遍历文件
                {
            int l=strlen(argv[i]);
            if (argv[i][l-1]!='/')
            {
                strcpy(path,argv[i]);
                path[l]='/';
                path[l+1]='\0';
            }
            else
            {
                strcpy(path,argv[i]);
                path[l]='\0';
            }
            work_dir(path,flag);
        }
        else //是文件时直接打印
        {
            display_type(path,flag);
        }
    }
        
}