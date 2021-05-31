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
    struct sockaddr_in addr;
    int socket_fd;
    socket_fd=socket(AF_INET,SOCK_STREAM,0);;
    if (socket_fd < 0)
    {
        my_error("socket",__LINE__);
    }
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(450);
    int e;
    e=connect(socket_fd, (struct sockaddr *)&addr,sizeof(addr));
    if (e==-1)
    my_error("connect",__LINE__);
    printf("ip:%s  port: %u\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
    write(socket_fd,"happy",6);
    close(socket_fd);
}