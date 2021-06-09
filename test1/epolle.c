#include "ddd.h"
#include <poll.h>
       #define _GNU_SOURCE         /* See feature_test_macros(7) */
       #include <signal.h>
int main()
{
    struct pollfd fdds[1024];  
    char buf[100];
    struct sockaddr_in addr,ss;
    int socket_fd,r_fd;
    socklen_t ss_len=sizeof(ss);
    socket_fd=Socket(AF_INET,SOCK_STREAM,0);;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(4500);
    int e;
    int opt=1;
    setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    Bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    if ((e=listen(socket_fd,128))==-1)
    {
        my_error("socket_fd",__LINE__);
    }
    int maxfd=0;
    int selet_ret;
    maxfd=0;
    fdds[0].fd=socket_fd;
    fdds[0].events=POLLIN;
    int j=0;
    for (j=1;j<1024;j++)
    {
        fdds[j].fd=-1;
    }
    int i;
    while(1)
    {
        selet_ret=poll(fdds,maxfd+1,-1);
        if(selet_ret<0)
        {
            my_error("select",__LINE__);
            exit(1);
        }
        if (fdds[0].revents&POLLIN)
        {
            r_fd=accept(socket_fd,(struct sockaddr *)&ss,&ss_len);
            printf("ip:%s  port: %u\n",inet_ntoa(ss.sin_addr),ntohs(ss.sin_port));
            for (i=1;i<1024;i++)
            {
                if (fdds[i].fd<0)
                {
                    fdds[i].fd=r_fd;
                    fdds[i].events=POLLIN;
                    break;
                }
            }
            if (r_fd>maxfd)
            {
                maxfd=r_fd;
            }
            if (--selet_ret<0)
            {
                continue;
            }
        }
        for(i=1;i<=maxfd;i++)
        {
            if (fdds[i].fd==-1)
            {
                continue;
            }
            if (fdds[i].revents&POLLIN)
            {
                int ret;
                ret=read(fdds[i].fd,buf,sizeof(buf));
                if (ret==0)
                {
                    close(i);
                    fdds[i].fd=-1;
                }
                else if (ret<0)
                {
                    my_error("read",__LINE__);
                }
                write(fdds[i].fd,buf,ret);
                write(STDOUT_FILENO,buf,ret);
            }
            if (--selet_ret<=0)
            {
                break;
            }

        }

    }
    close(socket_fd);
    return 0;
}