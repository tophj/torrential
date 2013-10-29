#ifndef TORRENTIAL_DOWNLOADS_SYSTEM_FUNCTION_WRAPPERS
#define TORRENTIAL_DOWNLOADS_SYSTEM_FUNCTION_WRAPPERS

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>

//Converts 64 bit numbers to network byte ordering (Big Endian)
uint64_t htonll(uint64_t value);

//Converts 64-bit numbers to host byte ordering (Little Endian)
uint64_t ntohll(uint64_t value);

int GetAddrInfo(const char * node, const char * service, const struct addrinfo * hints, struct addrinfo ** res);

int Socket(int domain, int type, int protocol);

int SetSockOpt(int sockFd, int level, int optName, const void * optVal, socklen_t optLen);

int Connect(int sockfd, struct sockaddr * serv_addr, int addrlen);

int Bind(int sockFd, struct sockaddr * myAddr, int addrLen);

int Select(int nfds, fd_set * readFds, fd_set * writeFds, fd_set * exceptFds, struct timeval * timeout);

ssize_t SendTo(int socket, const void * message, size_t length, 
	int flags, const struct sockaddr * destAddr, socklen_t destLen);

ssize_t RecvFrom(int sockFd, void * buffer, size_t length, int flags, 
	struct sockaddr * address, socklen_t * addressLength);

int Close(int fd);

#endif