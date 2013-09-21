#ifndef TORRENTIAL_DOWNLOADS_SYSTEM_FUNCTION_WRAPPERS
#define TORRENTIAL_DOWNLOADS_SYSTEM_FUNCTION_WRAPPERS

#include <sys/socket.h>
#include <iostream>

int Socket(int domain, int type, int protocol);

int Connect(int sockfd, struct sockaddr * serv_addr, int addrlen);

ssize_t SendTo(int socket, const void * message, size_t length, 
	int flags, const struct sockaddr * destAddr, socklen_t destLen);

#endif