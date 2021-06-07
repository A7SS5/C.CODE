#include "ddd.h"
void my_error(const char *error_string, int line) //错误处理函数
{
    fprintf(stderr, "line:%d",line);
    perror(error_string);
    exit(1);
}
int Socket(int domain, int type, int protocol)
{
    int ret;
    ret = socket(domain, type, protocol);
    if (ret < 0)
    {
        my_error("socket",__LINE__);
        exit(1);
    }
    else return ret;
}
int Connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen)
{
    int ret =connect(sockfd,addr,addrlen);
    if (ret<0)
    {
        my_error("connect",__LINE__);
        exit(1);
    }
    else return 0;
}
int Bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen)
{
    int ret=bind(sockfd,my_addr,addrlen);
    if (ret < 0)
    {
        my_error("bind",__LINE__);
        exit(1);
    }
}
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    int ret=accept(s,addr,addrlen);
    if (ret < 0)
    {
        my_error("accpet",__LINE__);
        exit(1);
    }
    else return ret;
}
