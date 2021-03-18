#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#define PARAM_NONE 0
#define PARAM_A 1
#define PARAM_L 2
int max_long = 0;
int shengyu = 100;
#define MAXhang 100
void my_error(const char *err_string, int line)
{

    fprintf(stderr, "line:%d", line);
    perror(err_string);
    
}
void display_attribute(struct stat buf, char *name)
{
    struct passwd *p;
    struct group *g;
    g = getgrgid(buf.st_gid);
    p = getpwuid(buf.st_uid);
    if (g==NULL||p==NULL)
    {   printf("无法打开文件\n");
        perror("getpwuid");
        return;
    }
    if (S_ISLNK(buf.st_mode))
    {
        printf("l");
    }
    else if (S_ISREG(buf.st_mode))
    {
        printf("_");
    }
    else if (S_ISDIR(buf.st_mode))
    {
        printf("d");
    }
    else if (S_ISCHR(buf.st_mode))
    {
        printf("c");
    }
    else if (S_ISBLK(buf.st_mode))
    {
        printf("b");
    }
    else if (S_ISFIFO(buf.st_mode))
    {
        printf("f");
    }
    else if (S_ISSOCK(buf.st_mode))
    {
        printf("s");
    }
    if (buf.st_mode & S_IRUSR)
    {
        printf("r");
    }
    else
        printf("-");
    if (buf.st_mode & S_IWUSR)
    {
        printf("w");
    }
    else
        printf("-");
    if (buf.st_mode & S_IXUSR)
    {
        printf("x");
    }
    else
        printf("-");

    if (buf.st_mode & S_IRGRP)
    {
        printf("r");
    }
    else
        printf("-");
    if (buf.st_mode & S_IWGRP)
    {
        printf("w");
    }
    else
        printf("-");
    if (buf.st_mode & S_IXGRP)
    {
        printf("x");
    }
    else
        printf("-");

    if (buf.st_mode & S_IROTH)
    {
        printf("r");
    }
    else
        printf("-");
    if (buf.st_mode & S_IWOTH)
    {
        printf("w");
    }
    else
        printf("-");
    if (buf.st_mode & S_IXOTH)
    {
        printf("x");
    }
    else
        printf("-");
    printf(" ");
    printf("%ld", buf.st_nlink);
    printf(" ");
    printf("%-5s", p->pw_name);
    printf("%-5s", g->gr_name);
    printf("%6ld", buf.st_size);
    printf(" ");
    int size = strlen(ctime(&buf.st_ctime));
    char timering[size + 1];
    strcpy(timering, ctime(&buf.st_ctime));
    timering[size - 1] = '\0';
    printf("%s", timering);
    printf(" ");
    printf("%s", name);
    printf("\n");
}

void simple(char *name)
{
    if (shengyu < max_long)
    {
        printf("\n");
        shengyu = MAXhang;
    }
    printf("%s", name);
    int i = 0;
    int j = max_long - strlen(name);
    for (i = 0; i < j; i++)
    {
        printf(" ");
    }
    printf("  ");
    shengyu -= (max_long + 2);
}
void display(int flag, char *name)
{
    int sb=flag;
    if (flag==4|| flag==5||flag==7||flag==6)
    {
        flag-=4;
    }
    int i = 0, j = 0;
    char a[PATH_MAX + 1];
    struct stat buf;

    for (i = 0, j = 0; i < strlen(name); i++)
    {
        if (name[i] == '/')
        {
            j = 0;
            continue;
        }
        a[j++] = name[i];
    }
    a[j] = '\0';

    if (lstat(name, &buf) == -1)
    {

        perror("lstat");
    }

    if (flag == 0)
    {
        if (a[0] != '.')
        {
            simple(a);
        }
    }
    else if (flag == 1)
    {
        simple(a);
    }
    else if (flag == 2)
    {
        if (a[0] != '.')
        {
            display_attribute(buf, a);
        }
    }
    else if (flag == 3)
    {
        display_attribute(buf, a);
    }
    if (sb!=flag)
    {
        flag=sb;
    }
}
void display_dir(int flag, char *name)
{

    struct stat buf;
    DIR *dir;
    struct dirent *ptr;
    int num = 0;
    int i;
    
    
      
    if ((dir = opendir(name)) == NULL)
    {
        
        perror("opendir");
        return;   
    }
    int sp[2];
    int n = 0;
    int len = strlen(name);
    while ((ptr = readdir(dir)) != NULL)
    {
        if (strcmp(ptr->d_name, "..") == 0 || strcmp(ptr->d_name, ".") == 0)
        {
            sp[n] = num;
            n++;
        }

        

        num++;
        if (strlen(ptr->d_name) > max_long)
        {
            max_long = strlen(ptr->d_name);
        }
    }
    closedir(dir);
    char filenames[num][max_long+len+1];
     if ((dir = opendir(name)) == NULL)
    {
        
        perror("opendir");
        return;   
    }
    printf("\n\n");
    printf("%s:\n", name);
    i=0;
    while ((ptr = readdir(dir)) != NULL)
    {
        strncpy(filenames[i], name, len);
        filenames[i][len] = '\0';
        strcat(filenames[i], ptr->d_name);
        filenames[i][len + strlen(ptr->d_name)] = '\0';
        i++;
    }
    for (i = 0; i < num; i++)
    {
        display(flag, filenames[i]);
        
    }
    closedir(dir);
    if (flag==5||flag == 4 || flag == 6 || flag == 7)
    {

        for (i = 0; i < num; i++)
        {
            if (i == sp[0] || i == sp[1])
            {
                continue;
            }
            
            if (lstat(filenames[i], &buf) == -1)
            {    printf("%s\n", filenames[i]);
                my_error("stat", __LINE__);
            }
            if (S_ISDIR(buf.st_mode)&&!S_ISLNK(buf.st_mode))
            {
              
                int l=strlen(filenames[i]);
                if (filenames[i][l - 1] != '/')
                {
                    filenames[i][l] = '/';
                    filenames[i][l + 1] = '\0';
                }
                

                display_dir(flag, filenames[i]);
                
            }  
            
        }
    }
}

int main(int argc, char **argv)
{
    char path[PATH_MAX + 1];
    struct stat buf;
    int flag = 0;
    char command[100];
    int count = 0;
    int i = 1;
    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {

            strcat(command, argv[i]);
            count += 1;
        }
    }
    int judge_a = 0;
    int judge_R = 0;
    int judge_l = 0;
    for (i = 0; i < strlen(command); i++)
    {
        if (command[i] == '-')
        {
            continue;
        }
        else if (command[i] == 'a' && judge_a == 0)
        {
            flag += 1;
            judge_a = 1;
        }
        else if (command[i] == 'R' && judge_R == 0)
        {
            flag += 4;
            judge_R = 1;
        }
        else if (command[i] == 'l' && judge_l == 0)
        {
            flag += 2;
            judge_l = 1;
        }
    }
    if ((count + 1) == argc)
    {
        strcat(path, "./");
        path[2] = '\0';
        display_dir(flag, path);
        return 0;
    }

    i = 1;
    do
    {
        if (argv[i][0] == '-')
        {
            i++;
            continue;
        }
        else
        {
            strcpy(path, argv[i]);
            if (stat(path, &buf) == -1)
                my_error("stat", __LINE__);
            if (S_ISDIR(buf.st_mode))
            {

                if (path[strlen(argv[i]) - 1] != '/')
                {
                    path[strlen(argv[i])] = '/';
                    path[strlen(argv[i]) + 1] = '\0';
                }
                else
                    path[strlen(argv[i])] = '\0';

                display_dir(flag, path);
                i++;
            }
            else
            {
                display(flag, path);
                i++;
            }
        }

    } while (i < argc);

    return 0;
}