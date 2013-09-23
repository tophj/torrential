#ifndef TORRENTIAL_DOWNLOADS_SYSTEM_FUNCTION_WRAPPERS
#define TORRENTIAL_DOWNLOADS_SYSTEM_FUNCTION_WRAPPERS

#include <sys/socket.h>
#include <iostream>
#include <unistd.h>

int Socket(int domain, int type, int protocol);

int SetSockOpt(int sockFd, int level, int optName, const void * optVal, socklen_t optLen);

int Connect(int sockfd, struct sockaddr * serv_addr, int addrlen);

int Bind(int sockFd, struct sockaddr * myAddr, int addrLen);

ssize_t SendTo(int socket, const void * message, size_t length, 
	int flags, const struct sockaddr * destAddr, socklen_t destLen);

ssize_t RecvFrom(int sockFd, void * buffer, size_t length, int flags, 
	struct sockaddr * address, socklen_t * addressLength);

int Close(int fd);

#endif