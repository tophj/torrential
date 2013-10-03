#ifndef TORRENTIAL_DOWNLOADS_SYSTEM_FUNCTION_WRAPPERS
#define TORRENTIAL_DOWNLOADS_SYSTEM_FUNCTION_WRAPPERS

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

//Converts 64 bit numbers to network byte ordering (Big Endian)
uint64_t htonll(uint64_t value);

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