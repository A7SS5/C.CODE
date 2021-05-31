#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
void my_error(const char *error_string, int line) //错误处理函数
{
    fprintf(stderr, "line:%d",line);
    perror(error_string);
    exit(1);
}
int main()
{
    char buf[100];
    struct sockaddr_in addr,ss;
    int socket_fd,r_fd;
    socklen_t ss_len=sizeof(ss);
    socket_fd=socket(AF_INET,SOCK_STREAM,0);;
    if (socket_fd < 0)
    {
        my_error("socket",__LINE__);
    }
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(450);
    int e;
    e=bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (e==-1)
    {
        my_error("bind",__LINE__);
    }
    if ((e=listen(socket_fd,4))==-1)
    {
        my_error("socket_fd",__LINE__);
    }
    r_fd=accept(socket_fd,(struct sockaddr *)&ss,&ss_len);
    if (r_fd < 0)
    {
        my_error("accept",__LINE__);
    }
    printf("ip:%s  port: %u\n",inet_ntoa(ss.sin_addr),ntohs(ss.sin_port));
    int ret;
    ret=read(r_fd,buf,sizeof(buf));
    write(STDOUT_FILENO,buf,ret);
    close(r_fd);
    close(socket_fd);
    return 0;
}