#include "SystemFunctionWrappers.h"


uint64_t htonll(uint64_t value) {

	const uint32_t high_part = htonl(value >> 32);
	const uint32_t low_part = htonl(value & 0xFFFFFFFF);
	return (((uint64_t) low_part) << 32) | (uint64_t) high_part;
}

uint64_t ntohll(uint64_t value) {

	const uint32_t high_part = ntohl(value >> 32);
	const uint32_t low_part = ntohl(value & 0xFFFFFFFF);
	return (((uint64_t) low_part) << 32) | (uint64_t) high_part;
}

int GetAddrInfo(const char * node, const char * service,
                const struct addrinfo * hints,
                struct addrinfo ** res) {

	int returnValue = -1;
	if ((returnValue = getaddrinfo(node, service, hints, res)) != 0) {

		std::cout << "The error is: " << gai_strerror(returnValue) << std::endl;
	}
	return returnValue;
}

int Socket(int domain, int type, int protocol) {

	int returnFileDescriptor = socket(domain, type, protocol);
	if (returnFileDescriptor == -1) {

		std::cout << "CALLING SOCKET RETURNED AN INVALID FILE DESCRIPTOR WITH ARGUMENTS: " 
			<< domain << ", " << type << ", " << protocol << std::endl;
	}
	return returnFileDescriptor;
}

int SetSockOpt(int sockFd, int level, int optName, const void * optVal, socklen_t optLen) {

	int returnValue = setsockopt(sockFd, level, optName, optVal, optLen);
	if (returnValue == -1) {

		std::cout << "CALLING SETSOCKOPT FAILED WITH ARGUMENTS: "
			<< sockFd << ", " << level << ", " << optName << ", " 
			<< optVal << ", " << optLen << std::endl;
	}
	return returnValue;
}

int Bind(int sockFd, struct sockaddr * myAddr, int addrLen) {

	int returnValue = bind(sockFd, myAddr, addrLen);
	if (returnValue == -1) {

		std::cout << "CALLING BIND FAILED WITH ARGUMENTS: "
			<< sockFd << ", " << myAddr << ", " << addrLen << std::endl;
	}
	return returnValue;
}

int Connect(int sockfd, struct sockaddr * serverAddress, int addressLength) {

	int returnValue = connect(sockfd, serverAddress, addressLength);
	if (returnValue == -1) {

		std::cout << "CALLING CONNECT FAILED WITH ARGUMENTS: " 
			<< sockfd << ", " << serverAddress << ", " << addressLength << std::endl;
	}
	return returnValue;
}

ssize_t RecvFrom(int sockFd, void * buffer, size_t length, int flags, 
	struct sockaddr * address, socklen_t * addressLength) {

	int returnValue = recvfrom(sockFd, buffer, length, flags, address, addressLength);
	if (returnValue == -1) {

		std::cout << "CALLING RECVFROM FAILED WITH ARGUMENTS: "
			<< sockFd << ", " << buffer << ", " << length
			<< flags << ", " << address << ", " << addressLength
			<< std::endl;
	}
	return returnValue;
}

ssize_t SendTo(int socket, const void * message, size_t length, 
	int flags, const struct sockaddr * destAddr, socklen_t destLen) {

	ssize_t numBytesSent = -1;
	if ((numBytesSent = sendto(socket, message, length, flags, destAddr, destLen)) == -1) {

		std::cout << "CALLING SENDTO FAILED WITH ARGUMENTS: " 
			<< socket << ", " << message << ", " << length
			<< ", " << flags << ", " << destAddr 
			<< ", " << destLen << std::endl;
	}
	return numBytesSent;
}

int Close(int fd) {

	int returnValue = close(fd);
	if (returnValue == -1) {

		std::cout << "CALLING CLOSE FAILED WITH ARGUMENTS: " << fd << std::endl; 
	}
	return returnValue;	
}