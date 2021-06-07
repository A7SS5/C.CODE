#ifndef _DDD_H_
#define _DDD_H_
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/time.h>
void my_error(const char *error_string, int line); //错误处理函数
int Socket(int domain, int type, int protocol);
int Connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
int Bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
#endif